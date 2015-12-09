import QtQuick 2.0
import QtTest 1.1
import VeinEntity 1.0
import ServerSetup 1.0
import ClientSetup 1.0


Rectangle {
  id: root
  height: 100
  width: 100

  readonly property int entityId: 100
  readonly property string entityName : "TestEntity";
  readonly property int timeout: 500 //msecs

  readonly property var testObject: {
    "name" : "testObject",
        "x" : 20,
        "y" : 40,
        "velocity": Qt.vector2d(45, 99),
        "size": 1.25,
        "properties": ["20", 5, 2.333, false]
  }

  readonly property var testData: [
    //basic types
    {tag: "bool", value: true},
    {tag: "int", value: 100},
    {tag: "double", value: 55.321},
    {tag: "string", value: "test_data_string"},
    {tag: "vector", value: Qt.vector2d(10,20)},
    //lists
    {tag: "int_list", value: [10,-20, 40, -80, 160]},
    {tag: "double_list", value: [11.5E16, -23.0, 34.5, -33.33, 44.44E-8]},
    {tag: "string_list", value: ["red_123", "blue_123", "yellow_123", "green_123", "purple_123"]},
    {tag: "vector_list", value: [Qt.vector2d(10,20), Qt.vector2d(510.23, 453.1), Qt.vector2d(0.000054, -123.321)]},
  ]

  ServerSetup {
    Component.onCompleted: {
      initData(entityId);
    }
    onSigReady: {
      clientSetup.initData(entityId)
    }
  }
  ClientSetup {
    id: clientSetup
    onSigReady: {
      VeinEntity.setRequiredIds([100])
    }
  }



  TestCase {
    id: unitTests
    name: "unittests"
    when: VeinEntity.state===VeinEntity.VQ_LOADED
    property QtObject tmpEntity;

    function initTestCase() {
      tmpEntity = VeinEntity.getEntity(root.entityName)
    }


    function test_000_entity() {
      compare((typeof tmpEntity) !== undefined, true, "TestEntity not available")
      compare(tmpEntity.entityId(), root.entityId, "Wrong entity id")
      compare(tmpEntity.keys().length, 3, "Component count != 3") // order is nondeterministic so compare the count
    }

    function test_001_component_data() {
      return root.testData
    }

    function test_001_component(data) {
      tmpEntity.testVariant=data.value;
      tryCompare(tmpEntity, "testVariant", data.value, root.timeout)
    }

    function test_002_object() {
      tmpEntity.testObject = root.testObject
      tryCompare(tmpEntity, "testObject", root.testObject, root.timeout)

      //comparing object identity/equality is an implementation detail blackmagic of QML TestCase so this test is rather non obvious
      //to validate the correctness of this test, compare to some other object to let the test fail
      expectFailContinue("", "This check verifies that the test can actually fail")
      compare(tmpEntity, "testObject", {"name": "wrong"})
    }

    function test_003_negativeEntity() {
      warn("ignore next warning about 'No entity found...'")
      var someVar = VeinEntity.getEntity("nondescript")
      compare(someVar, null, "Returned invalid entity")
    }
  }

  TestCase {
    name: "benchmarks"
    when: unitTests.completed

    property QtObject tmpEntity;

    function initTestCase() {
      tmpEntity = VeinEntity.getEntity(root.entityName)
    }

    function benchmark_000_getEntity() {
      var someVar = VeinEntity.getEntity(root.entityName)
    }

    function benchmark_001_setComponent_data() {
      return root.testData
    }

    function benchmark_001_setComponent(data) {
      tmpEntity.testVariant= 0
      tmpEntity.testVariant=data.value
      tryCompare(tmpEntity, "testVariant", data.value, root.timeout)
    }

    function benchmark_002_setObject() {
      tmpEntity.testObject = 0
      tmpEntity.testObject = root.testObject
      tryCompare(tmpEntity, "testObject", root.testObject, root.timeout)
    }
  }
}

