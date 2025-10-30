#include "ControlPanel.h"
#include "ui_ControlPanel.h"
#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include "AlgorithmController.h"

ControlPanel::ControlPanel(QWidget* parent)
    : QWidget(parent),
      ui(new Ui::ControlPanel)
{
    ui->setupUi(this);

    ui->algorithmComboBox->addItem("BFS", QVariant::fromValue(AlgorithmType::BFS));
    ui->algorithmComboBox->addItem("DFS", QVariant::fromValue(AlgorithmType::DFS));
    ui->algorithmComboBox->addItem("Dijkstra", QVariant::fromValue(AlgorithmType::Dijkstra));

    connect(ui->setupGraphButton, &QPushButton::clicked, this, &ControlPanel::setupGraphClicked);
    connect(ui->addVertexButton, &QPushButton::clicked, this, &ControlPanel::onAddVertexClicked);
    connect(ui->addEdgeButton, &QPushButton::clicked, this, &ControlPanel::onAddEdgeClicked);
    connect(ui->removeVertexButton, &QPushButton::clicked, this, &ControlPanel::onRemoveVertexClicked);
    connect(ui->removeEdgeButton, &QPushButton::clicked, this, &ControlPanel::onRemoveEdgeClicked);
    connect(ui->startButton, &QPushButton::clicked, this, &ControlPanel::onStartAlgorithmClicked);
    connect(ui->resetButton, &QPushButton::clicked, this, &ControlPanel::onResetClicked);
    connect(ui->nextButton, &QPushButton::clicked, this, &ControlPanel::nextClicked);
    connect(ui->prevButton, &QPushButton::clicked, this, &ControlPanel::prevClicked);

    connect(ui->playButton, &QPushButton::clicked, this, &ControlPanel::onPlayAlgorithmClicked);
    connect(ui->pauseButton, &QPushButton::clicked, this, &ControlPanel::onPauseAlgorithmClicked);
    connect(ui->speedSlider, &QSlider::valueChanged, this, &ControlPanel::onSpeedSliderChanged);
    connect(ui->algorithmComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ControlPanel::onAlgorithmChanged);

    setGraphEditingEnabled(false);
    setAlgorithmControlsEnabled(false);
    onSpeedSliderChanged(ui->speedSlider->value());
}

ControlPanel::~ControlPanel() { delete ui; }

void ControlPanel::setGraphEditingEnabled(bool enabled) {
    ui->graphEditBox->setEnabled(enabled);
}

void ControlPanel::setAlgorithmControlsEnabled(bool enabled) {
    ui->algorithmBox->setEnabled(enabled);
    ui->playerBox->setEnabled(enabled);
    if (enabled) {
         resetPlayerControls();
    }
}

void ControlPanel::updateStartVertexComboBox(const std::vector<Vertex_2*>& vertices) {
    ui->startVertexComboBox->clear();
    bool hasActive = false;
    for (auto v : vertices) {
        if (v && v->isActive()) {
            ui->startVertexComboBox->addItem(QString("Vertex %1").arg(QString::number(v->getId())), QVariant(v->getId()));
            hasActive = true;
        }
    }
    ui->algorithmBox->setEnabled(hasActive);
    ui->playerBox->setEnabled(hasActive);
    ui->startButton->setEnabled(hasActive);
}

void ControlPanel::resetPlayerControls() {
    ui->startButton->setEnabled(ui->startVertexComboBox->count() > 0);
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(false);
    ui->nextButton->setEnabled(false);
    ui->prevButton->setEnabled(false);
    ui->resetButton->setEnabled(false);

    ui->setupGraphButton->setEnabled(true);
    ui->graphEditBox->setEnabled(true);
    ui->algorithmBox->setEnabled(true);
}

void ControlPanel::onStartAlgorithmClicked() {
    if (ui->startVertexComboBox->count() == 0) return;

    int startId = ui->startVertexComboBox->currentData().toInt();

    ui->setupGraphButton->setEnabled(false);
    ui->graphEditBox->setEnabled(false);
    ui->algorithmBox->setEnabled(false);
    ui->startButton->setEnabled(false);

    ui->playButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->nextButton->setEnabled(true);
    ui->prevButton->setEnabled(false);
    ui->resetButton->setEnabled(true);

    emit startAlgorithmClicked(startId);
}

void ControlPanel::onPauseAlgorithmClicked() {
    ui->playButton->setEnabled(true);
    ui->pauseButton->setEnabled(false);
    ui->nextButton->setEnabled(true);
    ui->prevButton->setEnabled(true);

    emit pauseAlgorithmClicked();
}

void ControlPanel::onPlayAlgorithmClicked() {
    ui->playButton->setEnabled(false);
    ui->pauseButton->setEnabled(true);
    ui->nextButton->setEnabled(false);
    ui->prevButton->setEnabled(false);
    emit playAlgorithmClicked();
}


void ControlPanel::onResetClicked() {
    resetPlayerControls();
    emit resetClicked();
}

void ControlPanel::setupGraphClicked() {
    emit setupGraphButtonClicked();
}

void ControlPanel::onAlgorithmChanged(int index) {
    AlgorithmType type = ui->algorithmComboBox->itemData(index).value<AlgorithmType>();
    emit algorithmChanged(type);
}

void ControlPanel::onSpeedSliderChanged(int value) {
    double minp = 0;
    double maxp = 100;
    double minv = log(50);
    double maxv = log(2000);
    double scale = (maxv - minv) / (maxp - minp);
    double speed = exp(minv + scale * (maxp - value));
    emit speedChanged(static_cast<int>(speed));
}

QPushButton* ControlPanel::getNextButton() const { return ui->nextButton; }
QPushButton* ControlPanel::getPrevButton() const { return ui->prevButton; }
QPushButton* ControlPanel::getPlayButton() const { return ui->playButton; }
QPushButton* ControlPanel::getPauseButton() const { return ui->pauseButton; }
QPushButton* ControlPanel::getStartButton() const { return ui->startButton; }

void ControlPanel::onAddVertexClicked() { emit addVertexClicked(); }
void ControlPanel::onAddEdgeClicked() { emit addEdgeClicked(); }
void ControlPanel::onRemoveVertexClicked() { emit removeVertexClicked(); }
void ControlPanel::onRemoveEdgeClicked() { emit removeEdgeClicked(); }