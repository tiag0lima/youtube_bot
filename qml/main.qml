import QtQuick 2.0
import QtQuick.Controls 1.0

Item {
	id: main
	width: widget.getSize().width
	height: widget.getSize().height


	function loadUrl(idd, url) {
		//listview.contentItem.children[idd].setUrl(url)
	}

	function getUrl(idd) {
		return listview.contentItem.children[idd].getUrl()
	}

	function runJavaScript(idd, js) {
		listview.contentItem.children[idd].runJavaScript(js)
	}


	/*
	CustomMenu {
		width: parent.width
		height: parent.height
	}*/
	
	
	ListView {
		id: listview
		model: widget.lenWatcher
		/*
		y: 150
		width: parent.width
		height: parent.height-150
		*/
		z: -1
		anchors.fill: parent
	

		delegate: WView {
			id: wview
			width: listview.width
			height: listview.getDelegateHeight()

			Component.onCompleted: {
				if (! widget.isEditorMode)
					videoWatcher.goNextVideo(index)
			}

			onLoaded: function() { 
				if (widget.isEditorMode)
					videoEditor.onLoaded()
				else
					videoWatcher.onLoaded(index) 
			}
			onJavaScriptFinished: function (result) { 
				if (widget.isEditorMode)
					videoEditor.onJavaScriptFinished(result)
				else
					videoWatcher.onJavaScriptFinished(index, result)
			}

		}

		function getDelegateHeight() {
			if (listview.height < 200)
				return listview.height
			else if (listview.count*200 < listview.height)
				return listview.height/listview.count
			else
				return 200
		}
	}
	
	
}
