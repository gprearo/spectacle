/*
 *  Copyright (C) 2015 Boudhayan Gupta <me@BaloneyGeek.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 */

#ifndef KSCORE_H
#define KSCORE_H

#include <QUrl>
#include <QFile>
#include <QTemporaryFile>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QDateTime>
#include <QImageWriter>
#include <QMimeDatabase>
#include <QMimeType>
#include <QStandardPaths>
#include <QFileDialog>
#include <QPrinter>
#include <QPainter>
#include <QRect>
#include <QIcon>
#include <QDir>
#include <QDrag>
#include <QMimeData>
#include <QClipboard>
#include <QTimer>
#include <QMetaObject>
#include <QDebug>

#include <KLocalizedString>
#include <KJob>
#include <KRun>
#include <KService>
#include <KConfigGroup>
#include <KSharedConfig>
#include <KWindowSystem>
#include <KMessageBox>
#include <KNotification>
#include <KIO/FileCopyJob>
#include <KIO/StatJob>

#include "PlatformBackends/ImageGrabber.h"
#include "PlatformBackends/X11ImageGrabber.h"
#include "PlatformBackends/DummyImageGrabber.h"

#include "Gui/KSMainWindow.h"
#include "Config.h"

class KSCore : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString filename READ filename WRITE setFilename NOTIFY filenameChanged)
    Q_PROPERTY(bool overwriteOnSave READ overwriteOnSave WRITE setOverwriteOnSave NOTIFY overwriteOnSaveChanged)
    Q_PROPERTY(ImageGrabber::GrabMode grabMode READ grabMode WRITE setGrabMode NOTIFY grabModeChanged)
    Q_PROPERTY(QString saveLocation READ saveLocation WRITE setSaveLocation NOTIFY saveLocationChanged)

    public:

    explicit KSCore(bool backgroundMode, ImageGrabber::GrabMode grabMode, QString &saveFileName, qint64 delayMsec, bool sendToClipboard, bool notifyOnGrab, QObject *parent = 0);
    ~KSCore();

    QString filename() const;
    void setFilename(const QString &filename);
    ImageGrabber::GrabMode grabMode() const;
    void setGrabMode(const ImageGrabber::GrabMode grabMode);
    bool overwriteOnSave() const;
    void setOverwriteOnSave(const bool overwrite);
    QString saveLocation() const;
    void setSaveLocation(const QString &savePath);

    signals:

    void errorMessage(const QString errString);
    void allDone();
    void filenameChanged(QString filename);
    void grabModeChanged(ImageGrabber::GrabMode mode);
    void overwriteOnSaveChanged(bool overwriteOnSave);
    void saveLocationChanged(QString savePath);
    void imageSaved(QUrl location);

    public slots:

    void takeNewScreenshot(ImageGrabber::GrabMode mode, int timeout, bool includePointer, bool includeDecorations);
    void showErrorMessage(const QString errString);
    void screenshotUpdated(const QPixmap pixmap);
    void screenshotFailed();
    void doStartDragAndDrop();
    void doPrint(QPrinter *printer);
    void doGuiSaveAs();
    void doAutoSave();
    void doSendToService(KService::Ptr service);
    void doSendToOpenWith();
    void doSendToClipboard();

    private:

    QUrl getAutoSaveFilename();
    QString makeTimestampFilename();
    QString makeSaveMimetype(const QUrl url);
    bool writeImage(QIODevice *device, const QByteArray &format);
    bool localSave(const QUrl url, const QString mimetype);
    bool remoteSave(const QUrl url, const QString mimetype);
    bool tempFileSave();
    QUrl tempFileSave(const QString mimetype);
    bool doSave(const QUrl url);
    bool isFileExists(const QUrl url);
    QUrl getTempSaveFilename() const;

    bool             mBackgroundMode;
    bool             mNotify;
    bool             mOverwriteOnSave;
    bool             mBackgroundSendToClipboard;
    QPixmap          mLocalPixmap;
    QString          mFileNameString;
    QUrl             mFileNameUrl;
    ImageGrabber    *mImageGrabber;
    KSMainWindow    *mMainWindow;
};

#endif // KSCORE_H