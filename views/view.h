/**************************************************************************************
**
** Copyright (C) 2014 Files Drag & Drop
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public
** License along with this library; if not, write to the Free Software
** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**
**************************************************************************************/

#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QList>
#include <QMenu>
#include <QLabel>
#include <QMovie>
#include <QListWidgetItem>
#include <QSystemTrayIcon>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>

#include "deviceview.h"
#include "widget.h"
#include "dialogs/settingsdialog.h"
#include "historyelement.h"
#include "historyelementview.h"
#include "dialogs/aboutdialog.h"
#include "historygripbutton.h"
#include "centerinfowidget.h"
#include "../model.h"
#include "../helpers/servicehelper.h"
#include "service.h"
#include "dialogs/updatedialog.h"
#include "overlaymessagedisplay.h"
#include "progressindicator.h"

namespace Ui {
class View;
}

/**
  * @class View
  *
  * MVC main view
  */
class View : public QMainWindow
{
    Q_OBJECT
    
public:
    /**
      * Constructor
      *
      * @param model MVC model
      */
    explicit View(Model *model);
    /**
      * Destructor
      */
    ~View();

    /**
      * Find the proper position for each device depending on the list size
      *
      * @param size Device list size
      * @return List of positions (x, y)
      */
    QList<QPair<unsigned, unsigned> > getPosition(unsigned size);
    /**
      * Clear the view
      */
    void clearGrid();
    /**
      * CLear the history
      */
    void clearHistory();
    /**
     * Clear animation from memory
     */
    void clearAnimations();
    /**
      * Find a graphical device by its name
      *
      * @param uid Device UID
      * @return Device searched or NULL if not found
      */
    DeviceView* getDeviceByUID(const QString& uid) const;
    /**
      * Create actions for the tray icon menu
      */
    void createTrayActions();
    /**
      * Create actions for the history context menu
      */
    void createContextMenuActions();
    /**
      * Create the tray icon menu and the tray itself
      */
    void createTrayIcon();
    /**
      * Update the status bar, recount all the devices unavailable for fill the transferts running
      */
    void updateStatusBar();
    /**
      * Enable the status bar and fill the text properly
      */
    void enableStatusBar();
    /**
      * Disable the status bar (mean 0 transferts running)
      */
    void disableStatusBar();
    /**
      * Try to open a file or a link from an history item
      *
      * @param item History Item
      */
    void openActionHistoryItem(QListWidgetItem *item);
    /**
     * Initialize animations states
     */
    void initAnimations();
    /**
     * Reset all the positions for animations.
     * Called each left size, this method allow the animation to fit the window size
     */
    void resetLeftSlidePositions();
    /**
     * Reset all the positions for animations.
     * Called each right size, this method allow the animation to fit the window size
     */
    void resetRightSlidePositions();
    /**
      * Delete the central information widget if needed
      */
    void clearCenterInfoWidget();
    /**
     * Init the status
     */
    void initStatusbar();
    /**
      * Display the corresponding error for the bonjour service
      */
    void displayBonjourServiceError();
    /**
      * Setter : _lastBonjourState
      */
    void setBonjourState(BonjourServiceState state);
    /**
     * @overload QWidget::focusInEvent
     */
    void focusInEvent(QFocusEvent *);
    /**
     * Show a message in the tooltip of the tray
     *
     * @param message Message to display on the balloon tooltip of the tray
     */
    void showTrayMessage(const QString &message);

signals:
    /**
      * Link to the controller
      *
      * @param name The name of the device
      * @param urls List of urls
      * @param type Action to do on data reception
      */
    void sendFile(const QString &name, const QList<QUrl> &urls, DataType type);
    /**
      * Link to the controller
      *
      * @param name The name of the device
      * @param string The string to send
      * @param type Action to do on data reception
      */
    void sendText(const QString &name, const QString &string, DataType type);
    /**
      * Notify the controller for register the service
      */
    void registerService();
    /**
      * Notify the controller for unregister the service
      */
    void unregisterService();
    /**
      * Notify the controller for register the service again
      */
    void serviceNameChanged();
    /**
      * Notify the service for an history element deleted
      */
    void deleteFromHistory(int row);
    /**
      * Notify the service for an history clean up
      */
    void clearHistoryTriggered();
    /**
     * Notify the controller for a transfert interruption
     */
    void cancelTransfert(const QString &uid);
    /**
      * Notify the controller for a incoming transfert interruption
      */
    void cancelIncomingTransfert();
    /**
      * Notify the controller that the window is focused to ask for new records
      */
    void focused();

public slots:
    /**
      * SLOT : On service name changed in the settings, notify the controller.
      */
    void onServiceNameChanged();
    /**
      * SLOT : Refresh the view after a setting change
      */
    void onRefreshDevicesAvailability();
    /**
      * SLOT : Handle the history view
      * Triggered on settings changed
      */
    void refreshHistoryView();
    /**
      * On device unavailable (when a connection is running)
      *
      * @param uid The device UID
      * @param state State of the last transfert
      */
    void onDeviceUnavailable(const QString &uid, TransfertState state);
    /**
      * On device available (when a connection isn't running)
      *
      * @param uid The device UID
      * @param state State of the last transfert
      */
    void onDeviceAvailable(const QString &uid, TransfertState state);
    /**
     * On device progress changed
     *
     * @param message Message to display
     * @param uid The device UID
     * @param progress The progress of the upload as a percentage
     */
    void onProgressUpdated(const QString &message, const QString &uid, unsigned progress);
    /**
     * Show a message
     */
    void onFileTooBig();
    /**
      * Update the tray tooltip text and the action info (_infoAction) text
      */
    void updateTrayTooltip();
    /**
     * Refresh all the elements of the history
     **/
    void refreshAllHistory();
    /**
     * Show or hide the hiostory
     */
    void slideHistory();
    /**
      * SLOT : On service error, display an error box and close the service if necessary
      */
    void onServiceError(ServiceErrorState error, bool isCritical);
    /**
      * update the devices on the view
      */
    void updateDevices();
    /**
      * On window show request
      * Also make the windows as the focus
      */
    void onShow();
    /**
     * On file received
     *
     * @param fileName Name of the file
     * @param fileSize Size of the file
     */
    void onReceivingFile(const QString &fileName, int fileSize);
    /**
     * On folder received
     *
     * @param folderName Name of the folderSize
     * @param folderSize Size of the folder
     */
    void onReceivingFolder(const QString &folderName, int folderSize);
    /**
     * On url received
     *
     * @param url Url received
     */
    void onReceivingUrl(const QString &url);
    /**
     * On text received
     *
     * @param text Text received
     */
    void onReceivingText(const QString &text);
    /**
      * Show widget if needed
      * (only if it's enabled in settings and if there is at least one device)
      */
    void manageWidgetVisibility();

private slots:
    /**
      * On quit menu button trigered
      */
    void on_actionQuitter_triggered();
    /**
      * On file dropped on a device
      *
      * @param uid The UID of the device
      * @param urls List of urls
      * @param type Action to do on data reception
      */
    void onSendFile(const QString &uid, const QList<QUrl> &urls, DataType type);
    /**
      * On file dropped on a device
      *
      * @param uid The UID of the device
      * @param string The string to send
      * @param type Action to do on data reception
      */
    void onSendText(const QString &uid, const QString &string, DataType type);
    /**
      * @overload close event to keep the program in the tray bar
      */
    void closeEvent(QCloseEvent *event);
    /**
      * @overload QWidget::resizeEvent
      */
    void resizeEvent(QResizeEvent *event);
    /**
      * On trayicon activated
      *
      * Defines the actions relatives to the tray clicks
      */
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    /**
      * On about Qt requested
      */
    void on_action_propos_Qt_triggered();
    /**
      * On about requested
      */
    void on_action_propos_triggered();
    /**
     * On settings requested
     */
    void on_actionParam_tres_triggered();
    /**
     * On service requested
     */
    void on_actionService_triggered();
    /**
     * On service tray action triggered
     */
    void onServiceTriggered();
    /**
     * On settings requested from tray
     */
    void onSettingsActionTriggered();
    /**
      * On open file action triggered
      */
    void onHistoryOpenActionTriggered();
    /**
     * On clipboard action triggered
     */
    void onClipboardActionTriggered();
    /**
     * On open download history triggered
     */
    void onOpenDownloadFolderTriggered();
    /**
     * On delete from history triggered
     */
    void onDeleteFromHistoryTriggered();
    /**
     * On delete from disk triggered
     */
    void onDeleteFromDiskTriggered();
    /**
      * On clear history triggered
      */
    void onClearHistoryTriggered();
    /**
      * On history item double clicked
      */
    void on_historyView_itemDoubleClicked(QListWidgetItem *item);
    /**
     * On show download folder clicked, open the download folder in the explorer/finder
     */
    void on_openDownloadFolderButton_clicked();
    /**
     * On tray icon disabled on the settings dialog
     */
    void onTrayDisabled();
    /**
     * On tray icon enabled on the settings dialog
     */
    void onTrayEnabled();

public slots:
    /**
     * On service started
     */
    void startService();
    /**
     * On service stopped
     */
    void stopService();
    /**
     * SLOT : on history changed
     * @param history New history value
     */
    void onHistoryChanged(const QList<HistoryElement> &history);
    /**
     * SLOT : Download (on last history element) progress changed
     *
     * @param progress Download progress percentage
     */
    void historyElementProgressUpdated(unsigned progress);
    /**
     * Show the history menu
     */
    void onHistoryViewContextMenuRequested(const QPoint &pos);
    /**
     * SLOT : Triggered on left animation finished
     */
    void onLeftAnimationFinished();
    /**
     * SLOT : Triggered on right animation finished
     */
    void onRightAnimationFinished();
    /**
     * SLOT : Triggered when a new version is available
     */
    void onUpdateNeeded(const QString &version, const QString &note);
    /**
     * SLOT : On cancel transfert requested by user
     */
    void onCancelTransfert(const QString &uid);
    /**
     * SLOT : On incoming transfert canceled by user
     */
    void onCancelIncomingTransfert();
    /**
     * SLOT : On display message requested from device
     *
     * @param message Type Type of message
     * @param message Message
     */
    void onDisplayMessage(MessageType messageType, const QString &message);
    /**
     * SLOT : It will show the sliding widget (use to display the animated history)
     */
    void showSlidingWidget();

private:
    /// Main GUI
    Ui::View *ui;
    /// MVC Model
    Model *_model;
    /// Graphical device list
    QList<DeviceView*> _devices;
    /// Tray icon
    QSystemTrayIcon *_trayIcon;
    /// Tray icon menu
    QMenu *_trayIconMenu;
    /// Quit Action
    QAction *_quitAction;
    /// Settings action
    QAction *_settingsAction;
    /// Open Action
    QAction *_openAction;
    /// Info Action
    QAction *_infoAction;
    /// Service Action
    QAction *_serviceAction;
    /// Frameless widget on window closes
    Widget *_widget;
    /// About dialog of the soft
    AboutDialog _aboutDialog;
    /// Handle settings
    SettingsDialog _settingsDialog;
    /// Update dialog
    UpdateDialog _updateDialog;
    /// Labl for animated icon the status bar
    QLabel _statusLabel;
    /// Animated icon of the status bar
    QMovie _gif;
    /// Text of the status bar
    QLabel _statusText;
    /// Context menu of the history view
    QMenu _contextMenu;
    /// History element correponding to the right click on the history view
    QListWidgetItem *_rightClickHistoryElement;
    /// Open the corresponding history view element
    QAction *_historyOpenAction;
    /// Open the current download folder
    QAction *_historyOpenDownloadFolder;
    /// Information for an history element
    QAction *_historyInfo;
    /// Open the corresponding history view file
    QAction *_deleteFromHistory;
    /// Delete the corresponding file from the disk
    QAction *_deleteFromDisk;
    /// Copy the current history text into the clipboard
    QAction *_historyClipboardCopy;
    /// CLear the entire history
    QAction *_clearHistory;
    /// Number of running transferts
    unsigned _transfertsRunning;
    /// Widget used to display informations
    CenterInfoWidget *_infoWidget;
    /// Display button for the history
    HistoryGripButton _historyGripButton;
    /// History label animation
    QPropertyAnimation *_slidingWidgetAnimation;
    /// History button animation
    QPropertyAnimation *_HistoryButtonAnimation;
    /// Devices view animation
    QPropertyAnimation *_devicesViewAnimation;
    /// Parallel animaiton for all components
    QParallelAnimationGroup *_slideAnimation;
    /// Last bonjour state
    BonjourServiceState _lastBonjourState;
    /// Message displayed on the top of the screen
    OverlayMessageDisplay *_overlayMessageDisplay;
    /// Timer for tray message
    QTimer _trayTimer;
    /// Procedural animation for the status bar
    ProgressIndicator _progressIndicator;

    /**
     * Manage the font for the various OS
     */
    void manageFonts();
    /**
     * Update the tray icon with a grey or colored icon depending
     * on the number of devices detected
     */
    void updateTrayIcon();
    /**
     * Manage the right click on an history element containing text or URL
     *
     * @param historyElement Aimed element
     */
    void manageTextUrlHistoryContextMenu(HistoryElementView *historyElement);
    /**
     * Manage the right click on an history element containing a file or a folder
     *
     * @param historyElement Aimed element
     */
    void manageFileHistoryContextMenu(HistoryElementView *historyElement);
};

#endif // VIEW_H
