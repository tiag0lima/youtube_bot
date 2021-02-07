#include "VideoEditor.h"
#include <unistd.h>
#include <QMessageBox>
#include <QWidget>


VideoEditor::VideoEditor(QObject *parent)
	: QObject(parent)
{

}


void VideoEditor::setRootObject(QObject *rootObject) {
	mRootObject = rootObject;
}


void VideoEditor::loadUrl(QString url) {
	QMetaObject::invokeMethod(mRootObject, "loadUrl", Q_ARG(QVariant, 0), Q_ARG(QVariant, url));
}


void VideoEditor::runJavaScript(QString js) {
	QMetaObject::invokeMethod(mRootObject, "runJavaScript", Q_ARG(QVariant, 0), Q_ARG(QVariant, js));
}


void VideoEditor::editVideo(QString videoid) {
	QString url = tr("https://studio.youtube.com/video/%1/edit").arg(videoid);
	mLoadStep = VideoEditor::LOADSTEPS::LOAD_EDIT;
	loadUrl(url);
}


void VideoEditor::loadVideosId() {
	QString url = "https://m.youtube.com/feed/library";
	mLoadStep = VideoEditor::LOADSTEPS::LOAD_LIST;
	loadUrl(url);
}

void VideoEditor::signin() {
	QString url = "https://accounts.google.com/signin/v2/signinchooser?continue=https://m.youtube.com";
	mLoadStep = VideoEditor::LOADSTEPS::LOAD_SIGNIN;
	loadUrl(url);
}


void VideoEditor::onLoaded() {
	switch (mLoadStep) {
		case VideoEditor::LOADSTEPS::LOAD_SIGNIN: {
			checkSigned();	
			break;
		}
		case VideoEditor::LOADSTEPS::LOAD_EDIT: {
			continueEditVideo();
			break;
		}
		case VideoEditor::LOADSTEPS::LOAD_LIST: {
			continueLoadVideosId();
			break;
		}

	}

}

void VideoEditor::onJavaScriptFinished(QVariant var) {
	switch (mJsStep) {
		case VideoEditor::JSSTEPS::JS_GETTING_INFO: {
			getInfo(var);	
			break;
		}
		case VideoEditor::JSSTEPS::JS_EDITING_INFO: {
			editingVideo(var);
			break;
		}
	}

}



void VideoEditor::checkSigned() {
	QVariant var;
	QMetaObject::invokeMethod(mRootObject, "getUrl", Q_RETURN_ARG(QVariant, var), Q_ARG(QVariant, 0));
	if (not var.value<QString>().startsWith("https://https://accounts.google.com")) {
		mLoadStep = VideoEditor::LOADSTEPS::LOAD_NONE;
		emit onSigined();

	}
}


void VideoEditor::continueEditVideo() {
	mLoadStep = VideoEditor::LOADSTEPS::LOAD_NONE;
	mJsStep = VideoEditor::JSSTEPS::JS_GETTING_INFO;
	mGetInfoStep = VideoEditor::GETINFOSTEPS::CONTINUE_TO_STUDIO;
	getInfo(0);	
}


void VideoEditor::continueLoadVideosId() {
	mLoadStep = VideoEditor::LOADSTEPS::LOAD_NONE;
	mJsStep = VideoEditor::JSSTEPS::JS_GETTING_INFO;
	mGetInfoStep = VideoEditor::GETINFOSTEPS::GO_TO_LIBRARY;
	getInfo(0);
}


void VideoEditor::continueToStudio() {
	mGetInfoStep = VideoEditor::GETINFOSTEPS::WAIT_LOAD_STUDIO;
	runJavaScript("document.getElementsByClassName('continue-to-studio')[0].click()");
}


void VideoEditor::getInfo(QVariant var) {
	switch (mGetInfoStep) {
		case VideoEditor::GETINFOSTEPS::GO_TO_LIBRARY: {
			goToLibrary();
			break;
		}
		case VideoEditor::GETINFOSTEPS::GET_VIDEOS_ID: {
			getVideosId(var);
			break;
		}
		case VideoEditor::GETINFOSTEPS::CONTINUE_TO_STUDIO: {
			continueToStudio();		
			break;
		}
		case VideoEditor::GETINFOSTEPS::WAIT_LOAD_STUDIO: {
			waitLoadStudio(var);	
			break;
		}
	}

}


void VideoEditor::goToLibrary() {
	mGetInfoStep = VideoEditor::GETINFOSTEPS::GET_VIDEOS_ID;
	mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::WAIT_LOAD_VIDEOS_ID;
	runJavaScript("document.getElementsByClassName('compact-link-endpoint')[1].click()");
}


void VideoEditor::getVideosId(QVariant var) {
	switch (mGetVideosIdStep) {
			
		case VideoEditor::GETVIDEOSIDSTEPS::WAIT_LOAD_VIDEOS_ID: {
			waitLoadVideosId(var);
			break;
		}
		case  VideoEditor::GETVIDEOSIDSTEPS::HAS_MORE: {
			hasMore();
			break;
		}
		case VideoEditor::GETVIDEOSIDSTEPS::SHOW_MORE: {
			showMore(var);
			break;
		}
		case VideoEditor::GETVIDEOSIDSTEPS::GET_VIDEOS_ID_LEN: {
			getVideosIdLen();
			break;
		}
		case VideoEditor::GETVIDEOSIDSTEPS::STORE_VIDEOS_ID_LEN: {
			storeVideosIdLen(var);
			break;
		}
		case VideoEditor::GETVIDEOSIDSTEPS::GET_VIDEO_ID: {
			getVideoId();
			break;
		}
		case VideoEditor::GETVIDEOSIDSTEPS::STORE_VIDEO_ID: {
			storeVideoId(var);
			break;
		}
	}
}



void VideoEditor::waitLoadVideosId(QVariant var) {
	if (var.value<QString>() != "PLAYLISTS") {
		sleep(1);
		runJavaScript("document.getElementsByClassName('scbrr-tab')[2].innerText");
	} else {
		mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::HAS_MORE;
		getVideosId(0);
	}

}


void VideoEditor::hasMore() {
	mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::SHOW_MORE;
	runJavaScript("document.getElementsByClassName('cbox')[6].innerText");
}


void VideoEditor::showMore(QVariant var) {
	if (var.value<QString>() == "MOSTRAR MAIS") {
		mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::HAS_MORE;
		runJavaScript("document.getElementsByClassName('cbox')[6].click();");
	} else {
		mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::GET_VIDEOS_ID_LEN;
		getVideosId(0);
	}
}

void VideoEditor::getVideosIdLen() {
	mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::STORE_VIDEOS_ID_LEN;
	runJavaScript("document.getElementsByClassName('compact-media-item-metadata-content').length");
}

void VideoEditor::storeVideosIdLen(QVariant var) {
	mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::GET_VIDEO_ID;
	mVideosIdLen = var.value<int>();
	if (mVideosIdLen == 0)
		QMessageBox::critical(new QWidget(), "Sorry", "You don't have any video in your channel.");
	else
		getVideosId(0);
}

void VideoEditor::getVideoId() {
	mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::STORE_VIDEO_ID;
	runJavaScript(tr("document.getElementsByClassName('compact-media-item-metadata-content')[%1].href").arg(mVideosIds.count()));
}

void VideoEditor::storeVideoId(QVariant var) {
	mGetVideosIdStep = VideoEditor::GETVIDEOSIDSTEPS::GET_VIDEO_ID;
	mVideosIds << var.value<QString>().split('=')[1];
	if (mVideosIds.count() == mVideosIdLen)
		emit onLoadVideosIdFinished(mVideosIds);
	else
		getVideosId(0);
}



void VideoEditor::waitLoadStudio(QVariant var) {
	if (var == "null") {
		sleep(1);
		runJavaScript("document.getElementsByClassName('ytcp-mention-textbox')[9].innerText");
	} else {
		mLoadStep = VideoEditor::LOADSTEPS::LOAD_NONE;
		mJsStep = VideoEditor::JSSTEPS::JS_EDITING_INFO;
		mEditInfoSteps = VideoEditor::EDITINFOSTEPS::GET_KID_MODE;
		editingVideo(0);
	}
}


void VideoEditor::editingVideo(QVariant var) {
	switch (mEditInfoSteps) {
		case VideoEditor::EDITINFOSTEPS::GET_KID_MODE: {
			getKidMode();
			break;
		}
		case VideoEditor::EDITINFOSTEPS::SET_KID_MODE: {
			setKidMode(var);
			break;
		}
		case VideoEditor::EDITINFOSTEPS::SAVE_EDIT: {
			saveEdit();
			break;
		}
		case VideoEditor::EDITINFOSTEPS::FINISH_EDIT: {
			emit onEditFinished();
			break;
		}
	}

}


void VideoEditor::getKidMode() {
	mEditInfoSteps = VideoEditor::EDITINFOSTEPS::SET_KID_MODE;
	runJavaScript("document.getElementsByName('MADE_FOR_KIDS')[0].getAttribute('aria-selected')");
}


void VideoEditor::setKidMode(QVariant var) {
	QString kidmode = "MADE_FOR_KIDS";
	if (var.value<QString>() == "true")
		kidmode = "NOT_"+kidmode;
	mEditInfoSteps = VideoEditor::EDITINFOSTEPS::SAVE_EDIT;
	runJavaScript(tr("document.getElementsByName('%1')[0].click()").arg(kidmode));
}


void VideoEditor::saveEdit() {
	mEditInfoSteps = VideoEditor::EDITINFOSTEPS::FINISH_EDIT;
	runJavaScript("document.getElementsByClassName('ytcp-video-details-section')[6].click()");
}



