#ifndef __VIDEO_WATCHER_H__
#define __VIDEO_WATCHER_H__


#include <QObject>
#include <QVariant>
#include <QVector>
#include <QMap>

class VideoWatcher : public QObject {
	Q_OBJECT

public:
	VideoWatcher(QObject *parent);
	
	void setRootObject(QObject*);
	void setVideosId(QStringList);
	void setStucked(bool);
signals:
	void videoViewStucked(QString);

public slots:
	Q_INVOKABLE void goNextVideo(QVariant);
	Q_INVOKABLE void onLoaded(QVariant);
	Q_INVOKABLE void onJavaScriptFinished(QVariant, QVariant);

private:
	void loadUrl(QVariant, QString);
	void runJavaScript(QVariant, QString);
	void waitGoNextVideo(QVariant, int);

	void getVideoTime(QVariant);
	void getVideoViews(QVariant);
	void getVideoId(QVariant);

	void storeTime(QVariant, QVariant);
	void storeView(QVariant, QVariant);
	void storeId(QVariant, QVariant);

	void appendOnMaxView(QVariant, int);
	void checkMaxViewEqual(QVariant);

	QStringList mVideosId;
	QObject *mRootObject;
	int mMaxEqual{10};
	int mVideosPos{0};
	int mStuckControl{0};
	bool mStucked{false};

	enum JS_STEP {
		VIDEO_TIME,
		VIDEO_VIEWS,
		VIDEO_ID,
		JS_NONE
	};

	enum {
		TIME,
		VIEW,
		ID
	};

	QMap<QVariant, QMap<int, QVariant>> mInfomations;
	QMap<QVariant, VideoWatcher::JS_STEP> mJsStep; 
	QMap<QVariant, QVector<int>> mViewsControl;
};



#endif
