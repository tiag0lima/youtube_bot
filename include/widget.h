#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <QWidget>
#include <QVariant>
#include <QVBoxLayout>
#include <QQuickWidget>
#include "VideoWatcher.h"
#include "VideoEditor.h"

class widget : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(int lenWatcher READ lenVideoWatcher WRITE setLenVideoWatcher NOTIFY lenVideoWatcherChanged)
	Q_PROPERTY(bool isEditorMode READ editorMode WRITE setEditorMode NOTIFY editorModeChanged)

public:
	widget(QWidget *w = NULL);
	
	Q_INVOKABLE QSize getSize();

	Q_INVOKABLE int lenVideoWatcher();
	Q_INVOKABLE void setLenVideoWatcher(int);
	Q_INVOKABLE bool editorMode();
	Q_INVOKABLE void setEditorMode(bool);

signals:
	void lenVideoWatcherChanged();
	void editorModeChanged();

private:
	void build();
	void configure();
	void setLayouts();


	QVBoxLayout *mVLayout;
	QQuickWidget *mQuick;
	QSize mSize;
	

	VideoWatcher *mVideoWatcher;
	VideoEditor *mVideoEditor;

	int mLenVideoWatcherBefore{6};
	int mLenVideoWatcher{0};
	bool mEditorMode{false};
};



#endif
