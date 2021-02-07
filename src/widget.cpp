#include "widget.h"
#include <QQmlContext>
#include <QQmlError>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>
#include <QQuickItem>
#include <QRect>
#include <unistd.h>

widget::widget(QWidget *w)
	: QWidget(w)
{
	build();
	configure();
	setLayouts();

}


void widget::build()
{
	mVLayout = new QVBoxLayout(this);
	mQuick = new QQuickWidget(this);
	mVideoWatcher = new VideoWatcher(this);
	mVideoEditor = new VideoEditor(this);
}


void widget::configure()
{
	mVideoWatcher->setVideosId(QStringList() << "lc4BpgD2Fd4");

	QRect rec = QApplication::desktop()->screenGeometry();
	mSize = QSize(rec.width(), rec.height());

	mQuick->setFixedSize(mSize);
	mQuick->setSource(QUrl(QStringLiteral("qrc:/main.qml")));
	mQuick->rootContext()->setContextProperty("widget", this);
	mQuick->rootContext()->setContextProperty("videoWatcher", mVideoWatcher);
	mQuick->rootContext()->setContextProperty("videoEditor", mVideoEditor);
	mVideoWatcher->setRootObject(mQuick->rootObject());
	mVideoEditor->setRootObject(mQuick->rootObject());

	QList<QQmlError> l = mQuick->errors();
        if (l.count() > 0)
        {
                QString errors;
                for (auto err : l)
                {
                        errors += err.description();
                        errors += "\n";
                }
                QMessageBox::warning(this, tr("%1 erros on qml's source").arg
(l.count()), errors);
        }

	connect (mVideoEditor, &VideoEditor::onSigined, [&]() {
		mVideoEditor->loadVideosId();
	});

	connect (mVideoEditor, &VideoEditor::onEditFinished, [&]() {
		mVideoWatcher->setStucked(false);
		setLenVideoWatcher(0);
		setEditorMode(false);
		sleep(3); //wait save
		setLenVideoWatcher(mLenVideoWatcherBefore);
	});

	connect (mVideoEditor, &VideoEditor::onLoadVideosIdFinished, [&](QStringList videosId) {
		mVideoWatcher->setVideosId(videosId);
		setLenVideoWatcher(0);
		sleep(1);
		setEditorMode(false);
		setLenVideoWatcher(mLenVideoWatcherBefore);
	});

	connect (mVideoWatcher, &VideoWatcher::videoViewStucked, [&](QString videoId) {
		mLenVideoWatcherBefore = mLenVideoWatcher;
		setLenVideoWatcher(0);
		setEditorMode(true);
		setLenVideoWatcher(1);
		mVideoEditor->editVideo(videoId);
	});


	setEditorMode(true);
	setLenVideoWatcher(1);
	mVideoEditor->signin();
}

void widget::setLayouts()
{

	mVLayout->addWidget(mQuick);
	setLayout(mVLayout);
	mVLayout->setMargin(0);
	mVLayout->setSpacing(0);
}


QSize widget::getSize()
{
	return mSize;
}


int widget::lenVideoWatcher() {
	return mLenVideoWatcher;
}

void widget::setLenVideoWatcher(int len) {
	mLenVideoWatcher = len;
	emit lenVideoWatcherChanged();
}

bool widget::editorMode() {
	return mEditorMode;
}

void widget::setEditorMode(bool em) {
	mEditorMode = em;
	emit editorModeChanged();
}
