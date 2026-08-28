pragma Singleton
import QtQuick

QtObject {
  property var stack

  function go(page, props = {}) {
    stack.push(page, props)
  }

  function back() {
    stack.pop()
  }
}
