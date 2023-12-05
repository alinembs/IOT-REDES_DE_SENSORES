
const themeSwitch = document.getElementById("btn_mode");
const body = document.getElementById("body");
const manual = document.getElementById("manual-m");

themeSwitch.addEventListener("click", () => {

  var xhr = new XMLHttpRequest();
  if (body.className === "eyes-off") {
    manual.style.display = "grid";
    body.className = "eyes-on";
    xhr.open("GET", "/update?state=1", true)

  } else if (body.className === "eyes-on") {
    manual.style.display = "none";
    body.className = "eyes-off";
    xhr.open("GET", "/update?state=0", true)
  }
  xhr.send();
});


///////////////////////////////////////////////////

const casa = document.getElementById("corpo-dados");
const config = document.getElementById("config");
const contact = document.getElementById("atuadores");
const about = document.getElementById("Add");

themeSwitch.addEventListener("click", () => {
  let currentBodyClass = casa.className;

  if (casa.className === "modo-1") {
    casa.className = "modo-2";
    config.className = "modo-2";
    contact.className = "modo-2";
    about.className = "modo-2";
  } else if (casa.className === "modo-2") {

    casa.className = "modo-1";
    config.className = "modo-1";
    contact.className = "modo-1";
    about.className = "modo-1";
  }
});
/////////////////////////////////////////////////
function openPage(pageName, elmnt, color) {

  var i, dados, tablinks;
  dados = document.getElementsByClassName("modo-1");

  if (dados.length < 4) {
    dados = document.getElementsByClassName("modo-2");
  }

  for (i = 0; i < dados.length; i++) {
    dados[i].style.display = "none";
  }
  tablinks = document.getElementsByClassName("tablink");
  for (i = 0; i < tablinks.length; i++) {
    tablinks[i].style.backgroundColor = "";
  }
  document.getElementById(pageName).style.display = "flex";
  elmnt.style.backgroundColor = color;
}

document.getElementById("defaultOpen").click();
/////////////////////////////////////////////////////////////////////////////////
function rangeSlide(value) {
  document.getElementById('rangeValue').innerHTML = value + "cm";
}
////////////////////////////////////////////////////////////////
var animateButton = function (e) {

  e.preventDefault;
  //reset animation
  e.target.classList.remove('animate');

  e.target.classList.add('animate');

  e.target.classList.add('animate');
  setTimeout(function () {
    e.target.classList.remove('animate');
  }, 6000);
};
//////////////////////////////////////////////////////////////////////
var classname = document.getElementsByClassName("button");

for (var i = 0; i < classname.length; i++) {
  classname[i].addEventListener('click', animateButton, false);
}

////////////////////////////////////////////////////////////////////////////////
// Get current sensor readings when the page loads
//window.addEventListener('load', getReadings);

// Create Temperature Chart
var chartT = new Highcharts.Chart({
  chart: { renderTo: 'chart-temperature' },
  title: { text: 'Gráfico de Temperatura em C°' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: {
      animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#059e8a' }
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Temperature (Celsius)' }
    //title: { text: 'Temperature (Fahrenheit)' }
  },
  credits: { enabled: false }
});


//////////////////////////////////////////////////////////////////////////////////
setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
      var x = (new Date()).getTime(),
        y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if (chartT.series[0].data.length > 40) {
        chartT.series[0].addPoint([x, y], true, true, true);
      } else {
        chartT.series[0].addPoint([x, y], true, false, true);
      }
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 5000);

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturef").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturef", true);
  xhttp.send();

}, 5000);
///////////////////////////////////////////////////////////

var chartH = new Highcharts.Chart({
  chart: { renderTo: 'chart-humidity' },
  title: { text: 'Gráfico de Humidade' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: {
      animation: false,
      dataLabels: { enabled: true }
    }
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second: '%H:%M:%S' }
  },
  yAxis: {
    title: { text: 'Humidity (%)' }
  },
  credits: { enabled: false }
});



setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("umidade").innerHTML = this.responseText;
      var x = (new Date()).getTime(),
        y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if (chartH.series[0].data.length > 40) {
        chartH.series[0].addPoint([x, y], true, true, true);
      } else {
        chartH.series[0].addPoint([x, y], true, false, true);
      }

    }
  };
  xhttp.open("GET", "/umidade", true);
  xhttp.send();
}, 5000);




////////////////////////////////////////////////////////////////
setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("servo").innerHTML = this.responseText;

    }
  };
  xhttp.open("GET", "/servo", true);
  xhttp.send();
}, 5000);
setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("solo").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/solo", true);
  xhttp.send();
}, 5000);

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("butt_bomba").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/butt_bomba", true);
  xhttp.send();

}, 5000);
function SubmitServo() {
  var positionS = document.getElementById("positionServo").value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value=" + positionS + "&", true);
  xhr.send();

}
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if (element.checked) { xhr.open("GET", "/bomba?relay=ON", true); }
  else { xhr.open("GET", "/bomba?relay=OFF", true); }
  xhr.send();
}
//////////////////////////////////////////////////////////

