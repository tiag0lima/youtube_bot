import QtQuick 2.9
import QtWebView 1.1



Item {
	id: root

	signal loaded()
	signal javaScriptFinished(var _result)

	function setUrl(url) {
		webview.url = url
	}

	function getUrl() {
		return webview.url
	}

	function runJavaScript(js) {
		webview.runJavaScript(js, function (result) {
			root.javaScriptFinished(result)
		})
	}


	WebView {
		id: webview
		anchors.fill: parent
		onLoadingChanged: {
			if (webview.loadProgress == 100) {
				timer.stop()
				timer.start()
			}
		}	
	}

	Timer {
		id: timer
		interval: 3000
		repeat: false
		onTriggered: {
			root.loaded()	
		}
	}
}
