#include "configpanel.h"
#include "ui_configpanel.h"
#include "appconfig.h"
#include "filehelper.h"
#include "settingsmanager.h"

#include <QFile>
#include <QDebug>
#include <QDir>
#include <QListWidget>

ConfigPanel::ConfigPanel(QWidget *parent) :
    QWidget(parent),
    _refreshMovie(ANIMATED_REFRESH_ICON),
    ui(new Ui::ConfigPanel)
{
    ui->setupUi(this);

    // Stylesheet
    QFile stylesheetFile(CONFIG_PANEL_CSS);
    if (stylesheetFile.open(QFile::ReadOnly)) {
        QString stylesheet = stylesheetFile.readAll();
        setStyleSheet(stylesheet);
    }

    // Manage history list widget
    createHistoryListWidget();

    // Connect events
    connect(&_refreshMovie, SIGNAL(frameChanged(int)), this, SLOT(updateRefreshFrame(int)));
}

ConfigPanel::~ConfigPanel()
{
    delete ui;
    delete _historyScrollBar;
}

void ConfigPanel::createHistoryListWidget() {
    _historyScrollBar = new TransparentScrollBar(ui->historyListWidget);

    for (int i = 0; i < 100; ++i) {
        ui->historyListWidget->addItem("test " + QString::number(i));
    }
}

void ConfigPanel::mousePressEvent(QMouseEvent *event) {
    if (ui->downloadFolderWidget->geometry().contains(event->pos())) {
        openDownloadFolder();
    } else if (ui->settingsWidget->geometry().contains(event->pos())) {
        qDebug() << "Settings";
    } else if (ui->aboutWidget->geometry().contains(event->pos())) {
        qDebug() << "About";
    }

    QWidget::mousePressEvent(event);
}

void ConfigPanel::openDownloadFolder() {
    QDir dlDir;

    dlDir.mkpath(SettingsManager::getDestinationFolder());
    FileHelper::openURL("file:///" + QDir::toNativeSeparators(SettingsManager::getDestinationFolder()));
}

void ConfigPanel::on_refreshButton_clicked()
{
    if (_refreshMovie.state() == QMovie::Running) {
        _refreshMovie.setPaused(true);
    } else if (_refreshMovie.state() == QMovie::Paused) {
        _refreshMovie.setPaused(false);
    } else {
        _refreshMovie.start();
    }
}

void ConfigPanel::updateRefreshFrame(int frame)
{
    Q_UNUSED(frame);
    ui->refreshButton->setIcon(QIcon(_refreshMovie.currentPixmap()));
}