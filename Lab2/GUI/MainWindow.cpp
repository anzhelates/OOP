#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ControlPanel.h"
#include "GraphScene.h"
#include "GraphSetupDialog.h"
#include "AddEdgeDialog.h"
#include "AdjacencyList_2.h"
#include "VertexItem.h"
#include "EdgeItem.h"

#include <QGraphicsView>
#include <QDockWidget>
#include <QMessageBox>
#include <QInputDialog>
#include <QPushButton>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      m_playSpeedMs(1000)
{
    ui->setupUi(this);

    m_controlPanel = new ControlPanel(this);

    QDockWidget* dockWidget = new QDockWidget("Control Panel", this);
    dockWidget->setWidget(m_controlPanel);
    dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    m_scene = new GraphScene(this);
    m_graphicsView = ui->graphicsView;
    m_graphicsView->setScene(m_scene);
    m_graphicsView->setRenderHint(QPainter::Antialiasing);
    m_graphicsView->setDragMode(QGraphicsView::NoDrag);
    m_graphicsView->setInteractive(true);
    m_graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    m_graphicsView->centerOn(0, 0);
    m_algoController = std::make_unique<AlgorithmController>(nullptr);
    m_playTimer = new QTimer(this);

    connect(m_controlPanel, &ControlPanel::setupGraphButtonClicked, this, &MainWindow::setupGraph);
    connect(m_controlPanel, &ControlPanel::addVertexClicked, this, &MainWindow::addVertex);
    connect(m_controlPanel, &ControlPanel::addEdgeClicked, this, &MainWindow::addEdge);
    connect(m_controlPanel, &ControlPanel::removeVertexClicked, this, &MainWindow::removeVertex);
    connect(m_controlPanel, &ControlPanel::removeEdgeClicked, this, &MainWindow::removeEdge);

    connect(m_controlPanel, &ControlPanel::algorithmChanged, this, &MainWindow::onAlgorithmChanged);
    connect(m_controlPanel, &ControlPanel::startAlgorithmClicked, this, &MainWindow::onStartAlgorithm);
    connect(m_controlPanel, &ControlPanel::playAlgorithmClicked, this, &MainWindow::onPlayClicked);
    connect(m_controlPanel, &ControlPanel::pauseAlgorithmClicked, this, &MainWindow::onPauseClicked);
    connect(m_controlPanel, &ControlPanel::nextClicked, this, &MainWindow::nextStep);
    connect(m_controlPanel, &ControlPanel::prevClicked, this, &MainWindow::prevStep);
    connect(m_controlPanel, &ControlPanel::resetClicked, this, &MainWindow::resetAlgorithm);
    connect(m_controlPanel, &ControlPanel::speedChanged, this, &MainWindow::onSpeedChanged);
    connect(m_playTimer, &QTimer::timeout, this, &MainWindow::nextStep);

    setCentralWidget(m_graphicsView);
    setWindowTitle("Algorithm Visualiser");
    setupGraph();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::createGraph(bool directed, bool weighted) {
    m_scene->clearScene();
    m_graph.reset();
    m_algoController->setGraph(nullptr);
    m_graph = std::make_unique<AdjacencyList_2>(directed, weighted);

    m_scene->setGraph(m_graph.get());
    m_algoController->setGraph(m_graph.get());

    updateGraphUI();
    m_controlPanel->setGraphEditingEnabled(true);
    m_controlPanel->setAlgorithmControlsEnabled(false);
}

void MainWindow::setupGraph() {
    onPauseClicked();

    GraphSetupDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        GraphSettings settings = dialog.getSettings();
        createGraph(settings.directed, settings.weighted);

        int vertexCount = dialog.getVertexCount();
        for (int i = 0; i < vertexCount; ++i) {
            std::string name = "v" + std::to_string(i);
            Vertex_2* newVertex = new Vertex_2(name);
            int id = m_graph->addVertex(newVertex);
            if (id != -1) {
                m_scene->addVertex(newVertex);
            } else {
                delete newVertex;
            }
        }

        auto edges = dialog.getEdges();
        for (const auto& edgeTuple : edges) {
            int fromId = std::get<0>(edgeTuple);
            int toId = std::get<1>(edgeTuple);
            double weight = m_graph->isWeighted() ? std::get<2>(edgeTuple) : 1.0;

            Vertex_2* v_from = m_graph->getVertexById(fromId);
            Vertex_2* v_to = m_graph->getVertexById(toId);

            if (v_from && v_to) {
                bool exists = m_graph->getEdge(fromId, toId) != nullptr;
                if (!m_graph->isDirected()) {
                    exists = exists || (m_graph->getEdge(toId, fromId) != nullptr);
                }
                if (exists) {
                    qWarning() << "Skipping duplicate edge from dialog:" << fromId << "->" << toId;
                    continue;
                }

                Edge_2* newEdge = new Edge_2(v_from, v_to, weight);
                m_graph->addEdge(newEdge);
                m_scene->addEdge(newEdge);
            } else {
                qWarning() << "Invalid edge from dialog" << fromId << "->" << toId;
            }
        }
        updateGraphUI();

    } else {
        if (!m_graph) {
            createGraph(false, false);
        }
    }
}

void MainWindow::addVertex() {
    onPauseClicked();
    if (!m_graph) return;

    std::string name = "v" + std::to_string(m_graph->getNumberOfVertices());
    Vertex_2* newVertex = new Vertex_2(name);

    int id = m_graph->addVertex(newVertex);
    if (id == -1) {
        delete newVertex;
        return;
    }
    m_scene->addVertex(newVertex);
    updateGraphUI();
    resetAlgorithm();
}

void MainWindow::addEdge() {
    onPauseClicked();
    if (!m_graph) return;

    int maxActiveId = -1;
    int activeVertices = 0;
     for(auto v : m_graph->getVertices()) {
        if (v && v->isActive()) {
            activeVertices++;
            maxActiveId = v->getId();
        }
    }

    if (activeVertices < 2) {
        QMessageBox::warning(this, "Error", "At least two active vertices needed");
        return;
    }

    AddEdgeDialog dialog(m_graph->isWeighted(), maxActiveId, this);
    if (dialog.exec() == QDialog::Accepted) {
        int fromId = dialog.getFromId();
        int toId = dialog.getToId();

        if (fromId < 0 || toId < 0) {
            QMessageBox::warning(this, "Error", "ID needed");
            return;
        }

        Vertex_2* v_from = m_graph->getVertexById(fromId);
        Vertex_2* v_to = m_graph->getVertexById(toId);

        if (!v_from || !v_to) {
            QMessageBox::warning(this, "Error", "Invalid ID");
            return;
        }

        if (fromId == toId) {
             QMessageBox::warning(this, "Error", "Knots are not allowed");
             return;
        }

        bool exists = m_graph->getEdge(fromId, toId) != nullptr;
        if (!m_graph->isDirected()) {
             exists = exists || (m_graph->getEdge(toId, fromId) != nullptr);
        }
        if (exists) {
             QMessageBox::warning(this, "Error", "Such edge already exists");
             return;
        }

        double weight = m_graph->isWeighted() ? dialog.getWeight() : 1.0;
        Edge_2* newEdge = new Edge_2(v_from, v_to, weight);

        m_graph->addEdge(newEdge);
        m_scene->addEdge(newEdge);
        updateGraphUI();
        resetAlgorithm();
    }
}

void MainWindow::removeVertex() {
    onPauseClicked();
    if (!m_graph) return;

    VertexItem* selectedItem = nullptr;
    for (QGraphicsItem* item : m_scene->selectedItems()) {
        selectedItem = qgraphicsitem_cast<VertexItem*>(item);
        if (selectedItem) break;
    }

    if (!selectedItem) {
        QMessageBox::information(this, "Remove Vertex", "Select a vertex to remove");
        return;
    }

    Vertex_2* vertexToRemove = selectedItem->getVertex();
    if (!vertexToRemove) return;

    selectedItem->removeEdges();
    for (Edge_2* edge : m_graph->getEdgesFrom(vertexToRemove->getId())) {
        m_graph->removeEdge(edge);
    }
    for (Vertex_2* v : m_graph->getVertices()) {
        if (v && v->isActive() && v->getId() != vertexToRemove->getId()) {
            Edge_2* e = m_graph->getEdge(v->getId(), vertexToRemove->getId());
            if (e) m_graph->removeEdge(e);
        }
    }

    m_graph->removeVertex(vertexToRemove->getId());
    m_scene->removeVertex(vertexToRemove);

    updateGraphUI();
    resetAlgorithm();
}

void MainWindow::removeEdge() {
    onPauseClicked();
    if (!m_graph) return;
    EdgeItem* selectedItem = nullptr;
    for (QGraphicsItem* item : m_scene->selectedItems()) {
        selectedItem = qgraphicsitem_cast<EdgeItem*>(item);
        if (selectedItem) break;
    }

    if (!selectedItem) {
        QMessageBox::information(this, "Remove edge", "Select an edge to remove");
        return;
    }

    Edge_2* edgeToRemove = selectedItem->getEdge();
    if (!edgeToRemove) return;

    m_scene->removeEdge(edgeToRemove);
    m_graph->removeEdge(edgeToRemove);

    updateGraphUI();
    resetAlgorithm();
}

void MainWindow::updateGraphUI() {
    if (!m_graph) return;

    m_controlPanel->updateStartVertexComboBox(m_graph->getVertices());
    m_scene->updateEdgeWeights();

    bool hasActiveVertices = false;
    for (auto v : m_graph->getVertices()) {
        if (v && v->isActive()) {
            hasActiveVertices = true;
            break;
        }
    }
    m_controlPanel->setAlgorithmControlsEnabled(hasActiveVertices);
}

void MainWindow::onAlgorithmChanged(AlgorithmType type) {
    m_algoController->setAlgorithm(type);
    resetAlgorithm();
}

void MainWindow::onStartAlgorithm(int startVertexId) {
    onPauseClicked();
    resetSceneStyles();
    m_algoController->reset();

    if (m_algoController->getAlgorithm() != AlgorithmType::BFS) {
        QMessageBox::warning(this, "Not available", "Currently only BFS is available");
        resetAlgorithm();
        return;
    }

    if (m_algoController->start(startVertexId, m_currentState)) {
        applyState(m_currentState);
        m_controlPanel->getPrevButton()->setEnabled(false);
        m_controlPanel->getNextButton()->setEnabled(true);
        m_controlPanel->getPlayButton()->setEnabled(true);
    } else {
        QMessageBox::warning(this, "Error", "The graph is empty or the starting vertex is not initialised");
        resetAlgorithm();
    }
}

void MainWindow::nextStep() {
    if (m_algoController->nextStep(m_currentState)) {
        applyState(m_currentState);
        m_controlPanel->getPrevButton()->setEnabled(true);
    } else {
        if (m_playTimer->isActive()) {
            onPauseClicked();
        }
        m_controlPanel->getNextButton()->setEnabled(false);
        m_controlPanel->getPlayButton()->setEnabled(false);
        QMessageBox::information(this, "Done", "The end");
    }
}

void MainWindow::prevStep() {
    if (m_playTimer->isActive()) {
        onPauseClicked();
    }

    if (m_algoController->prevStep(m_currentState)) {
        applyState(m_currentState);
        m_controlPanel->getNextButton()->setEnabled(true);
        m_controlPanel->getPlayButton()->setEnabled(true);
    }
    m_controlPanel->getPrevButton()->setEnabled(m_algoController->getCurrentStep() > 0);
}

void MainWindow::resetAlgorithm() {
    onPauseClicked();
    m_algoController->reset();
    resetSceneStyles();

    m_controlPanel->resetPlayerControls();

    if (m_graph) {
        m_controlPanel->updateStartVertexComboBox(m_graph->getVertices());
    }
}

void MainWindow::onPlayClicked() {
    if (m_algoController->getCurrentStep() == 0 && m_controlPanel->getStartButton()->isEnabled()) {
        QMessageBox::warning(this, "Error", "Press 'Start'");
        return;
    }
    m_playTimer->start(m_playSpeedMs);
}

void MainWindow::onPauseClicked() {
    m_playTimer->stop();
    if(m_controlPanel->getPauseButton()->isEnabled()) {
        m_controlPanel->getPlayButton()->setEnabled(true);
        m_controlPanel->getPauseButton()->setEnabled(false);
        m_controlPanel->getNextButton()->setEnabled(true);
        m_controlPanel->getPrevButton()->setEnabled(m_algoController->getCurrentStep() > 0);
    }
}

void MainWindow::onSpeedChanged(int speed) {
    m_playSpeedMs = speed;
    if (m_playTimer->isActive()) {
        m_playTimer->start(m_playSpeedMs);
    }
}

void MainWindow::applyState(const BFSState& state) {
    resetSceneStyles();

    const QColor visitedColor(Qt::darkGray);
    const QColor processingColor(Qt::yellow);
    const QColor currentEdgeColor(Qt::red);

    for (const EdgeId& id : state.visitedEdges) {
        Edge_2* edge = m_graph->getEdge(id.from, id.to);
        if (!edge && !m_graph->isDirected()) edge = m_graph->getEdge(id.to, id.from);

        EdgeItem* eItem = m_scene->getEdgeItem(edge);
        if (eItem) eItem->setColor(visitedColor);
    }

    for (int vertexId : state.visitedVertices) {
        Vertex_2* v = m_graph->getVertexById(vertexId);
        VertexItem* vItem = m_scene->getVertexItem(v);
        if (vItem) vItem->setColor(visitedColor);
    }

    for (const EdgeId& id : state.currentEdges) {
        Edge_2* edge = m_graph->getEdge(id.from, id.to);
        if (!edge && !m_graph->isDirected()) edge = m_graph->getEdge(id.to, id.from);

        EdgeItem* eItem = m_scene->getEdgeItem(edge);
        if (eItem) eItem->setColor(currentEdgeColor);
    }

    if (state.currentVertex != -1) {
        Vertex_2* v = m_graph->getVertexById(state.currentVertex);
        VertexItem* vItem = m_scene->getVertexItem(v);
        if (vItem) vItem->setColor(processingColor);
    }

}

void MainWindow::resetSceneStyles() {
    m_scene->resetAllColors();
}