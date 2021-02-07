#include "VideoWatcher.h"
#include <unistd.h>
#include <QProcess>
#include <QVariant>
#include <thread>

#include <QWidget>
#include <QMessageBox>

VideoWatcher::VideoWatcher(QObject *parent)
	: QObject(parent)
{
}



void VideoWatcher::setRootObject(QObject *obj) {
	mRootObject = obj;
}


void VideoWatcher::setVideosId(QStringList videosIds) {
	mVideosId = videosIds;	
}

void VideoWatcher::setStucked(bool stucked) {
	mViewsControl.clear();	
	mStuckControl += 1;
	mStucked = stucked;
}

void VideoWatcher::goNextVideo(QVariant idd) {
	mVideosPos = mVideosPos+1 >= mVideosId.count() ? 0 : mVideosPos+1;
	QString url = tr("https://m.youtube.com/watch?v=%1").arg(mVideosId[mVideosPos]);
	loadUrl(idd, url);
}


void VideoWatcher::loadUrl(QVariant idd, QString url) {
	QMetaObject::invokeMethod(mRootObject, "loadUrl", Q_ARG(QVariant, idd), Q_ARG(QVariant, url));
}


void VideoWatcher::runJavaScript(QVariant idd, QString js) {
	QMetaObject::invokeMethod(mRootObject, "runJavaScript", Q_ARG(QVariant, idd), Q_ARG(QVariant, js));
}


void VideoWatcher::onLoaded(QVariant idd) {
	getVideoTime(idd);
}

void VideoWatcher::onJavaScriptFinished(QVariant idd, QVariant var) {
	switch (mJsStep[idd]) {
		case VideoWatcher::JS_STEP::VIDEO_TIME: {
			storeTime(idd, var);
			break;
		}
		case VideoWatcher::JS_STEP::VIDEO_VIEWS: {
			storeView(idd, var);
			break;
		}
		case VideoWatcher::JS_STEP::VIDEO_ID: {
			storeId(idd, var);
			break;
		}
		case VideoWatcher::JS_STEP::JS_NONE: {
			QMessageBox::information(new QWidget(), "debug", var.value<QString>());
			break;
		}
	}

}

void VideoWatcher::getVideoTime(QVariant idd) {
	QString js;
	js = "document.getElementById('movie_player').getDuration()";
	mJsStep[idd] = VideoWatcher::JS_STEP::VIDEO_TIME;
	runJavaScript(idd, js);
}

void VideoWatcher::getVideoViews(QVariant idd) {
	QString js;
	js = "document.getElementsByClassName('secondary-text')[0].innerText";
	mJsStep[idd] = VideoWatcher::JS_STEP::VIDEO_VIEWS;
	runJavaScript(idd, js);
}

void VideoWatcher::getVideoId(QVariant idd) {
	QString js;
	js = "window.location.href";
	mJsStep[idd] = VideoWatcher::JS_STEP::VIDEO_ID;
	runJavaScript(idd, js);

}

void VideoWatcher::storeTime(QVariant idd, QVariant value) {
	mInfomations[idd][TIME] = value.value<int>();
	getVideoViews(idd);
}

void VideoWatcher::storeView(QVariant idd, QVariant value) {
	mInfomations[idd][VIEW] = value.value<QString>().split(' ')[0].toInt();
	getVideoId(idd);
}

void VideoWatcher::storeId(QVariant idd, QVariant value) {
	mInfomations[idd][ID] = value.value<QString>().split('=')[1];
	appendOnMaxView(mInfomations[idd][ID], mInfomations[idd][VIEW].value<int>());
	std::thread th(&VideoWatcher::waitGoNextVideo, this, idd, mInfomations[idd][TIME].value<int>());
	th.detach();
}


void VideoWatcher::waitGoNextVideo(QVariant idd, int delay) {
	int sc = mStuckControl;
	sleep(delay);
	if (not mStucked && sc == mStuckControl)
		goNextVideo(idd);
}



void VideoWatcher::appendOnMaxView(QVariant videoid, int views) {
	mViewsControl[videoid].push_back(views);
	while (mViewsControl[videoid].count() > mMaxEqual)
		mViewsControl[videoid].pop_front();
	checkMaxViewEqual(videoid);
}

void VideoWatcher::checkMaxViewEqual(QVariant videoid) {
	if (mViewsControl[videoid].count() < mMaxEqual)
		return;
	if (mViewsControl[videoid][0] == mViewsControl[videoid][mMaxEqual-1]) {
		emit videoViewStucked(videoid.value<QString>());
		mViewsControl[videoid].clear();
		mStucked = true;
		mStuckControl += 1;
	}
}


