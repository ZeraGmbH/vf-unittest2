import QtQuick 2.0
import QtTest 1.1
import VeinEntity 1.0
import TestSetup 1.0


Rectangle {
  id: root
  height: 100
  width: 100

  readonly property int entityId: 100
  readonly property string entityName : "TestEntity";
  readonly property int timeout: 5000 //msecs

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
      initData(entityId, entityName);
    }
    onSigReady: {
      clientSetup.initData()
    }
  }
  ClientSetup {
    id: clientSetup
    onSigReady: {
      VeinEntity.setRequiredIds([root.entityId])
    }
  }





  TestCase {
    id: unitTests
    name: "unittests"
    when: VeinEntity.state===VeinEntity.VQ_LOADED
    property QtObject tmpEntity;
    property var rpcCallback: ({});
    property bool rpcWasCalled: false;

    function initTestCase() {
      tmpEntity = VeinEntity.getEntity(root.entityName)
    }

    Connections {
      target: unitTests.tmpEntity
      onSigRPCFinished: {
        if(unitTests.rpcCallback[t_identifier] !== undefined)
        {
          unitTests.rpcCallback[t_identifier](t_identifier, t_resultData);
          delete unitTests.rpcCallback[t_identifier]; //remove the entry
        }
      }
    }


    function test_000_entity() {
      compare((typeof tmpEntity) !== undefined, true, "Entity not available")
      compare(tmpEntity.entityId(), root.entityId, "Wrong entity id")
      verify(tmpEntity.hasComponent("testVariant"), "No entity 'testVariant'")
      verify(tmpEntity.hasComponent("testObject"), "No entity 'testObject'")
      verify(tmpEntity.hasComponent("EntityName"), "No entity 'EntityName'")
      compare(tmpEntity.keys().length, 3, "Component count != 3") // sort order is nondeterministic so compare the count
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


      //comparing object identity/equality is an implementation detail blackmagic of QML TestCase, so this test is rather non obvious
      //to validate the correctness of this test, compare to some other object to let the test fail
      expectFailContinue("", "This check verifies that the test can actually fail")

      var tObject = root.testObject
      tObject.name = root.testObject.name+"_wrong"
      compare(tmpEntity.testObject, tObject, "This check verifies the test")
    }

    function test_003_negativeEntity() {
      ignoreWarning('No entity found with name: "nondescript"');
      var someVar = VeinEntity.getEntity("nondescript")
      compare(someVar, null, "Returned invalid non null entity")
    }

    function test_004_remoteProcedure() {
      var paramObject = { something: 10 }
      unitTests.rpcCallback[tmpEntity.invokeRPC("unitTestParamReturn()", paramObject)] = function (identifier, data) {
        unitTests.rpcWasCalled = true;
        compare(data["RemoteProcedureData::resultCode"], 0, "Test rpc call result code")
        compare(data["RemoteProcedureData::callParameters"].something, 10, "Test rpc call parameters")
        compare(data["RemoteProcedureData::callID"], identifier, "Test rpc call identifier")
      };
      tryCompare(unitTests, "rpcWasCalled", true, root.timeout, "Check if the rpc was actually called")
    }
  }

  TestCase {
    id: benchmarks
    name: "benchmarks"
    when: unitTests.completed

    property QtObject tmpEntity;
    property var rpcCallback: ({});
    property bool rpcWasCalled: false;

    Connections {
      target: benchmarks.tmpEntity
      onSigRPCFinished: {
        if(benchmarks.rpcCallback[t_identifier] !== undefined)
        {
          benchmarks.rpcCallback[t_identifier](t_identifier, t_resultData);
          delete benchmarks.rpcCallback[t_identifier]; //remove the entry
        }
      }
    }

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

    function  benchmark_003_testRPC() {
      var paramObject = { something: 10 }
      benchmarks.rpcCallback[tmpEntity.invokeRPC("unitTestParamReturn()", paramObject)] = function (identifier, data) {
        compare(data["RemoteProcedureData::resultCode"], 0, "Test rpc call result code")
        compare(data["RemoteProcedureData::callParameters"].something, 10, "Test rpc call parameters")
        compare(data["RemoteProcedureData::callID"], identifier, "Test rpc call identifier")
        benchmarks.rpcWasCalled = true;
      };
      tryCompare(benchmarks, "rpcWasCalled", true, root.timeout, "Check if the rpc was actually called")
    }
  }
}

