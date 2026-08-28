import QtQuick

Item {
  visible: searchService.dropdownOpened
  width: 320; height: 480
  y: 12

  ListView {
    anchors.fill: parent
    spacing: 4

    delegate: Row {
      anchors.fill: parent
      spacing: 6
    }
  }
}
