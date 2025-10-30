#pragma once

#include <QMainWindow>
#include <memory>
#include "AlgorithmController.h"
#include "AlgorithmStep.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QGraphicsView;
class ControlPanel;
class GraphScene;
class Graph_2;
class Vertex_2;
class Edge_2;
class QTimer;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    private slots:
        void setupGraph();
    void addVertex();
    void addEdge();
    void removeVertex();
    void removeEdge();

    void onAlgorithmChanged(AlgorithmType type);
    void onStartAlgorithm(int startVertexId);
    void nextStep();
    void prevStep();
    void resetAlgorithm();
    void onPlayClicked();
    void onPauseClicked();
    void onSpeedChanged(int speed);

private:
    void createGraph(bool directed, bool weighted);
    void updateGraphUI();

    void applyState(const BFSState& state);
    void resetSceneStyles();

    Ui::MainWindow* ui;
    ControlPanel* m_controlPanel;
    QGraphicsView* m_graphicsView;
    GraphScene* m_scene;

    std::unique_ptr<Graph_2> m_graph;
    std::unique_ptr<AlgorithmController> m_algoController;

    BFSState m_currentState;

    QTimer* m_playTimer;
    int m_playSpeedMs;
};