import QtQuick 2.0

Rectangle {
	id: root

	property var title: ""
	property var titleColor: "#222222"

	signal pressed()
	signal released()
	signal entered()
	signal exited()
	signal clicked()

	Text {
		id: titleField
		anchors.centerIn: parent
		text: root.title
		color: root.titleColor
		font {
			family: "Cursive"
		}
		verticalAlignment: Text.AlignVCenter
		horizontalAlignment: Text.AlignHCenter
	}

	MouseArea {
		anchors.fill: parent
		onClicked: root.clicked()
		onExited: root.exited()
		onEntered: root.entered()
	}
}
