
var spotifyPlayerToggled = false;

function hndlToggle(chkBox, togChkId, cacheVarName) {
  console.log("hndlToggle()");
  this[cacheVarName] = chkBox.checked;
  elementFromId = document.getElementById(togChkId);
  if (this[cacheVarName] === true) {
    elementFromId.style.display = "block";
  } else {
    elementFromId.style.display = "none";
  }
}


function auto_height(elem) {  /* javascript */
  elem.style.height = "30px";
  elem.style.height = (elem.scrollHeight) + "px";
}

function toggleGPSOLEDDisplay() {
  $.ajax({
    url: '/toggledoGPSOLEDDisplay',
    type: 'POST',
    dataType: "json",
    success: function () {
      console.log("toggleGPSOLEDDisplay");
    }
  });
}

function onValueRangeChange(value, labelId, prefix, affix) {
  document.querySelector(labelId).innerHTML = (prefix + value + affix);
}


//doPanTilt
$(document).ready(function () {
  $("#doPanTilt #doPanTiltForm").click(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/doPanTilt',
      data: $(this).serialize()
    });

  });
});


//textToSpeech
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


//oLEDDisplayTxt
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




//ultraSonicReading
$(document).ready(function () {
  $("#ultraSonicReading #ultraSonicReadingForm").submit(function (e) {
    e.preventDefault(); // avoid page refresh

    $.ajax({
      type: "POST",
      url: '/UltraSonicRequest',
      dataType: "json",
      success: function (ultrasonic_data) {
        if (ultrasonic_data.length > 0) {
          console.log("ultrasonic_data" + JSON.stringify(ultrasonic_data));
          document.getElementById('#ultraSonicReading #ultraSonicReadingForm #distanceContainer #ftextFieldDistance').value = 'UltraSonic done';
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