#ifndef __VIDEO_EDITOR_H__
#define __VIDEO_EDITOR_H__


#include <QObject>
#include <QVariant>

class VideoEditor : public QObject {
	Q_OBJECT
public:
	VideoEditor(QObject *parent=nullptr);


	void signin();
	void setRootObject(QObject *);
	void editVideo(QString);
	void loadVideosId();

signals:
	void onSigined();
	void onEditFinished();
	void onLoadVideosIdFinished(QStringList);

public slots:
	Q_INVOKABLE void onLoaded();
	Q_INVOKABLE void onJavaScriptFinished(QVariant);


private:
	
	void loadUrl(QString);
	void runJavaScript(QString);

	void checkSigned();
	void getInfo(QVariant);
	void continueEditVideo();
	void continueToStudio();
	void waitLoadStudio(QVariant);
	void editingVideo(QVariant);
	void getKidMode();
	void setKidMode(QVariant);
	void saveEdit();
	void continueLoadVideosId();
	void goToLibrary();
	void getVideosId(QVariant);
	void waitLoadVideosId(QVariant);
	void hasMore();
	void showMore(QVariant);
	void getVideosIdLen();
	void storeVideosIdLen(QVariant);
	void getVideoId();
	void storeVideoId(QVariant);

	
	QObject *mRootObject;
	int mVideosIdLen;
	QStringList mVideosIds;
	
	enum LOADSTEPS {
		LOAD_SIGNIN,
		LOAD_LIST,
		LOAD_EDIT,
		LOAD_NONE
	} mLoadStep;

	enum JSSTEPS {
		JS_GETTING_INFO,
		JS_EDITING_INFO,
		JS_NONE
	} mJsStep;

	enum GETINFOSTEPS {
		GO_TO_LIBRARY,
		GET_VIDEOS_ID,
		CONTINUE_TO_STUDIO,
		WAIT_LOAD_STUDIO
	} mGetInfoStep;

	enum GETVIDEOSIDSTEPS {
		WAIT_LOAD_VIDEOS_ID,
		HAS_MORE,
		SHOW_MORE,
		GET_VIDEOS_ID_LEN,
		STORE_VIDEOS_ID_LEN,
		GET_VIDEO_ID,
		STORE_VIDEO_ID
	} mGetVideosIdStep;

	enum EDITINFOSTEPS {
		GET_KID_MODE,
		SET_KID_MODE,
		SAVE_EDIT,
		FINISH_EDIT
	} mEditInfoSteps;
};


#endif
