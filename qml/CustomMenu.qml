import QtQuick 2.0


Rectangle {
	id: root 
	signal login()
	signal loadVideoList()
	signal startWatches()

	ListView {
		id: listView
		width: parent.width
		height: parent.height*.8
		spacing: parent.height*.01
		model: ListModel {
			ListElement {
				buttonTitle: "Login"
				whenClciked: function() {root.login()}
			}
			ListElement {
				buttonTitle: "Load Videos List"
				whenClciked: function() { root.loadVideoList() }
			}
			ListElement {
				buttonTitle: "set len watchers"
				whenClciked: function() { root.setLenWatchers() }
			}
			ListElement {
				buttonTitle: "Start watchers"
				whenClciked: function() { root.startWatchers() }
			}

		}

		delegate: CustomButton {
			id: button
			width: listView.width*.8
			height: listView.width*.1
			x: listView.width*.1
			radius: listView.width*.1
			title: buttonTitle
			color: "#fafafa"
			titleColor: "#212121"
			
			border.width: 1
			onClicked: whenClick()
			onPressed: {
				button.width = 0
				button.color = "#232323"
			}
			onExited: {
				button.border.width = 1
				button.color = "#212121"
			}
		}
	}

}

