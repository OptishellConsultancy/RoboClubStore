
$(document).ready(function () {
  $("#subTaskToggles #toggleSub #GPSOLEDPRNTToggle").checked = getGPSOLEDDisplay();
  $("#doPanTilt #doPanTiltForm #panContainer #pan").value = getCurrentPan();
  $("#doPanTilt #doPanTiltForm #tiltContainer #tilt").value = getCurrentTilt();
});

function hndlToggle(chkBox, togChkId, cacheVarName) {
  console.log("hndlToggle()");
  localStorage.setItem(cacheVarName) = chkBox.checked;
  elementFromId = $(togChkId);
  if (localStorage.getItem(cacheVarName) === true) {
    elementFromId.style.display = "block";
  } else {
    elementFromId.style.display = "none";
  }
}


function auto_height(elem) {  /* javascript */
  elem.style.height = "30px";
  elem.style.height = (elem.scrollHeight) + "px";
}
//GPSOLEDDisplay----------------------------------------------------------------

function toggleGPSOLEDDisplay() {
  $.ajax({
    url: '/toggleGPSOLEDDisplay',
    type: 'POST',
    dataType: "json",
    success: function (GPSOLEDDisplayData) {
      if (GPSOLEDDisplayData.length > 0) {
        console.log("GPSOLEDDisplayData" + JSON.stringify(GPSOLEDDisplayData));
        localStorage.setItem('GPSOLEDDisplayState', GPSOLEDDisplayData["nowState"]);
        $("#subTaskToggles #toggleSub #GPSOLEDPRNTToggle").checked = localStorage.getItem('GPSOLEDDisplayState')
      }
    }
  });
}


function getGPSOLEDDisplay() {
  if (localStorage.getItem('GPSOLEDDisplayState') === undefined) {
    $.ajax({
      url: '/getGPSOLEDDisplay',
      type: 'GET',
      dataType: "json",
      success: function (GPSOLEDDisplayData) {
        if (GPSOLEDDisplayData.length > 0) {
          console.log("getGPSOLEDDisplay" + JSON.stringify(GPSOLEDDisplayData));
          localStorage.setItem('GPSOLEDDisplayState', GPSOLEDDisplayData["nowState"]);
          return localStorage.getItem('GPSOLEDDisplayState')
        }
      }
    });
  } else {
    return localStorage.getItem('GPSOLEDDisplayState');
  }
}
function onValueRangeChange(value, labelId, prefix, affix) {
  document.querySelector(labelId).innerHTML = (prefix + value + affix);
}
//----------------------------------------------------------------

//doPanTilt----------------------------------------------------------------


$(document).ready(function () {
  
    $("#doPanTilt #doPanTiltForm").bind('click touchend', function(e){
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/doPanTilt',
      data: $(this).serialize(),
      success: function (getCurrentPanData) {
        if (getCurrentPanData.length > 0) {
          localStorage.setItem('CurrentPan', getCurrentPanData["CurrentPan"]);
          localStorage.setItem('CurrentTilt', getCurrentPanData["CurrentTilt"]);
        }
      }
    });

  });
});

function getCurrentPan() {
  if (localStorage.getItem('CurrentPan') === undefined) {
    $.ajax({
      url: '/getCurrentPan',
      type: 'GET',
      dataType: "json",
      success: function (getCurrentPanData) {
        if (getCurrentPanData.length > 0) {
          console.log("getCurrentPanData" + JSON.stringify(getCurrentPanData));
          localStorage.setItem('CurrentPan', getCurrentPanData[0]["CurrentPan"]);
          return localStorage.getItem('CurrentPan');
        }
      }
    });
  } else {
    return localStorage.getItem('CurrentPan');
  }
}
function getCurrentTilt() {
  if (localStorage.getItem('CurrentTilt') === undefined) {
    $.ajax({
      url: '/getCurrentTilt',
      type: 'GET',
      dataType: "json",
      success: function (getCurrentTiltData) {
        if (getCurrentTiltData.length > 0) {
          console.log("getCurrentTiltData" + JSON.stringify(getCurrentTiltData));
          console.log("CurrentTilt: " + getCurrentTiltData[0]["CurrentTilt"])
          localStorage.setItem('CurrentTilt', getCurrentTiltData[0]["CurrentTilt"]);
          return localStorage.getItem('CurrentTilt');
        }
      }
    });
  } else {
    return localStorage.getItem('CurrentTilt');
  }
}
//----------------------------------------------------------------

//textToSpeech----------------------------------------------------------------
$(document).ready(function () {
  $("#textToSpeech #textToSpeechForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/textToSpeech',
      data: $(this).serialize()
    });

  });
});

//----------------------------------------------------------------
//oLEDDisplayTxt----------------------------------------------------------------
$(document).ready(function () {
  $("#oLEDDisplayTxt #oledDisplayTextForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/writeOLEDText',
      data: $(this).serialize()
    });

  });
});

//----------------------------------------------------------------


//ultraSonicReading----------------------------------------------------------------
$(document).ready(function () {
  $("#ultraSonicReading #ultraSonicReadingForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/ultraSonicRequest',
      dataType: "json",
      success: function (ultrasonic_data) {
        if (ultrasonic_data.length > 0) {
          //console.log("ultrasonic_data" + JSON.stringify(ultrasonic_data));
          //
          //console.log("UltraSonicDistance: " + ultrasonic_data[0]["UltraSonicDistance"])
          dist = ultrasonic_data[0]["UltraSonicDistance"].replace("/r", "");
          temp = ultrasonic_data[0]["UltraSonicTemp"].replace("/r", "").replace("c", "");
          $("#ultraSonicReading #ultraSonicReadingForm #ftextDistance").val(dist);
          $("#ultraSonicReading #ultraSonicReadingForm #ftextTemp").val(temp);
        }
      }
    });

  });
});

function toggleOLEDUltraSonicDisplay() {
  $.ajax({
    url: '/toggleOLEDUltraSonicDisplay',
    type: 'POST',
    dataType: "json",
  });
  console.log("toggleOLEDUltraSonicDisplay")
}

//----------------------------------------------------------------
//AccMagReading----------------------------------------------------------------
$(document).ready(function () {
  $("#accMagReading #accMagReadingForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/accMagRequest',
      dataType: "json",
      success: function (accMag_data) {
        if (accMag_data.length > 0) {
          console.log("accMag_data" + JSON.stringify(accMag_data));
          //

          acc = accMag_data[0]["AccMagAcc"].replace("/r", "");
          mag = accMag_data[0]["AccMagMag"].replace("/r", "");
          heading = accMag_data[0]["AccMagHeading"].replace("/r", "");

          console.log("acc:" + acc)
          console.log("mag:" + mag)
          console.log("heading:" + heading)
          $("#accMagReading #accMagReadingForm #ftextAccel").val(acc);
          $("#accMagReading #accMagReadingForm #ftextMag").val(mag);
          $("#accMagReading #accMagReadingForm #ftextHeading").val(heading);
        }
      }
    });

  });
});

function toggleOLEDAccMagDisplay() {
  $.ajax({
    url: '/toggleOLEDAccMagDisplay',
    type: 'POST',
    dataType: "json",
  });
  console.log("toggleOLEDAccMagDisplay")
}

//----------------------------------------------------------------
