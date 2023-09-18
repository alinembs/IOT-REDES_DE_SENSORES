//Funções  pro Controle do Robô

//Controle da Bomba de Água
function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if (element.checked) {
    document.getElementById("btn_bomb").innerHTML = "Ligado";
    xhr.open("GET", "/bomba?relay=ON", true);
  } else {
    document.getElementById("btn_bomb").innerHTML = "Desligado";
    xhr.open("GET", "/bomba?relay=OFF", true);
  }
  xhr.send();
}

//Controle do Braço Robótico

function rangeSlide_base(value) {
  document.getElementById("rangeValue_base").innerHTML = value + "ºGraus";
}
function rangeSlide_vertical(value) {
  document.getElementById("rangeValue_vertical").innerHTML = value + "ºGraus";
}
function rangeSlide_horizontal(value) {
  document.getElementById("rangeValue_horizontal").innerHTML = value + "ºGraus";
}
function SubmitServo_1() {
  var positionS = document.getElementById("positionServo_1").value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/servo_base?value=" + positionS + "&", true);
  xhr.send();
}
function SubmitServo_2() {
  var positionS = document.getElementById("positionServo_2").value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/servo_vertical?value=" + positionS + "&", true);
  xhr.send();
}
function SubmitServo_3() {
  var positionS = document.getElementById("positionServo_3").value;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/servo_horizontal?value=" + positionS + "&", true);
  xhr.send();
}

//Funções para os Cards e os Gráficos

//Definições dos Charts - Gráficos

//GRAFICO 1
Highcharts.chart("chart_umidade", {
  chart: {
    type: "spline",
    animation: Highcharts.svg, // don't animate in old IE
    marginRight: 10,
    events: {
      load: function () {
        //----Pega do Sensor ---Manda o Valor da Umidade para o site

        var series = this.series[0];
        setInterval(function () {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("umidade").innerHTML = this.responseText;
              var x = new Date().getTime(); // current time
                // y = getRandomInt(1, 100);
                y = parseFloat(this.responseText);
              series.addPoint([x, y], true, true);
            }
          };
          xhttp.open("GET", "/umidade", true);
          xhttp.send();
        }, 5000);
      },
    },
  },

  time: {
    useUTC: false,
  },

  title: {
    text: "Gráfico",
  },
  accessibility: {
    announceNewData: {
      enabled: true,
      minAnnounceInterval: 15000,
      announcementFormatter: function (allSeries, newSeries, newPoint) {
        if (newPoint) {
          return "New point added. Value: " + newPoint.y;
        }
        return false;
      },
    },
  },

  xAxis: {
    type: "datetime",
    tickPixelInterval: 150,
  },

  yAxis: {
    title: {
      text: "Porcentagem",
    },
    plotLines: [
      {
        value: 0,
        width: 1,
        color: "#808080",
      },
    ],
  },

  tooltip: {
    headerFormat: "<b>{series.name}</b><br/>",
    pointFormat: "{point.x:%Y-%m-%d %H:%M:%S}<br/>{point.y:.2f}",
  },

  legend: {
    enabled: false,
  },

  exporting: {
    enabled: false,
  },

  series: [
    {
      name: "Umidade data",
      data: (function () {
        var data = [],
          time = new Date().getTime(),
          i;

        for (i = -19; i <= 0; i += 1) {
          data.push({
            x: time + i * 1000,

            y: 0,
          });
        }
        return data;
      })(),
    },
  ],
});

//---------------------------------------

//GRAFICO 2 --------------------------------
Highcharts.chart("chart_temperatura", {
  chart: {
    type: "spline",
    animation: Highcharts.svg, // don't animate in old IE
    marginRight: 10,
    events: {
      load: function () {
        //--Pega do Sensor---Manda o Valor da Temperatura para o site
        var series = this.series[0];
        setInterval(function () {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("temperaturec").innerHTML =
                this.responseText;
              y = parseFloat(this.responseText);

              var x = new Date().getTime(); // current time
              //y = getRandomInt(1, 100);

              series.addPoint([x, y], true, true);
            }
          };
          xhttp.open("GET", "/temperaturec", true);
          xhttp.send();
        }, 5000);
      },
    },
  },

  time: {
    useUTC: false,
  },

  title: {
    text: "Gráfico",
  },

  accessibility: {
    announceNewData: {
      enabled: true,
      minAnnounceInterval: 15000,
      announcementFormatter: function (allSeries, newSeries, newPoint) {
        if (newPoint) {
          return "New point added. Value: " + newPoint.y;
        }
        return false;
      },
    },
  },

  xAxis: {
    type: "datetime",
    tickPixelInterval: 150,
  },

  yAxis: {
    title: {
      text: "Temperatura em Cº",
    },
    plotLines: [
      {
        value: 0,
        width: 1,
        color: "#808080",
      },
    ],
  },

  tooltip: {
    headerFormat: "<b>{series.name}</b><br/>",
    pointFormat: "{point.x:%Y-%m-%d %H:%M:%S}<br/>{point.y:.2f}",
  },

  legend: {
    enabled: false,
  },

  exporting: {
    enabled: false,
  },

  series: [
    {
      name: "Temp data",
      data: (function () {
        var data = [],
          time = new Date().getTime(),
          i;

        for (i = -19; i <= 0; i += 1) {
          data.push({
            x: time + i * 1000,
            y: 0,
          });
        }
        return data;
      })(),
    },
  ],
});

//----------------------------------------------

//GRAFICO - 2

var chart_fluxo = Highcharts.chart("chart_fluxo", {
  chart: {
    type: "column",
  },
  title: {
    text: "Consumo de Água para Irrigação por cada Planta em Litros",
  },
  xAxis: {
    type: "Plantas_id",
    labels: {
      rotation: -45,
      style: {
        fontSize: "13px",
        fontFamily: "Verdana, sans-serif",
      },
    },
  },
  yAxis: {
    min: 0,
    title: {
      text: "Consumo(L)",
    },
  },
  legend: {
    enabled: false,
  },
  tooltip: {
    pointFormat: "Consumo de Água: <b>{point.y:.1f} L</b>",
  },
  series: [
    {
      name: "Plantas",
      colors: [
        "#9b20d9",
        "#9215ac",
        "#861ec9",
        "#7a17e6",
        "#7010f9",
        "#691af3",
        "#6225ed",
        "#5b30e7",
        "#533be1",
        "#4c46db",
        "#4551d5",
        "#3e5ccf",
        "#3667c9",
        "#2f72c3",
        "#277dbd",
        "#1f88b7",
        "#1693b1",
        "#0a9eaa",
        "#03c69b",
        "#00f194",
      ],
      colorByPoint: true,
      groupPadding: 0,
      data: [
        ["Planta_1", 37.33],
        ["Planta_2", 31.18],
        ["Planta_3", 27.79],
        ["Planta_4", 22.23],
        ["Planta_5", 21.91],
        ["Planta_6", 21.74],
        ["Planta_7", 21.32],
        ["Planta_8", 20.89],
        ["Planta_9", 20.67],
        ["Planta_10", 19.11],
        ["Planta_11", 16.45],
        ["Planta_12", 16.38],
        ["Planta_13", 15.41],
        ["Planta_14", 15.25],
        ["Planta_15", 14.974],
        ["Planta_16", 14.97],
        ["Planta_17", 14.86],
        ["Planta_18", 14.16],
        ["Planta_19", 13.79],
        ["Planta_20", 13.64],
      ],
      dataLabels: {
        enabled: true,
        rotation: -90,
        color: "#FFFFFF",
        align: "right",
        format: "{point.y:.1f}", // one decimal
        y: 10, // 10 pixels down from the top
        style: {
          fontSize: "13px",
          fontFamily: "Verdana, sans-serif",
        },
      },
    },
  ],
});
//---------------------------------------------------------

//Teste
// Gerar um número aleatório inteiro entre um mínimo e um máximo (inclusive)
function getRandomInt(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min;
}

//------Pega do Sensor----Manda o Valor do Fluxo para o site
setInterval(function () {
  var y = getRandomInt(1, 100);
  document.getElementById("fluxo").innerHTML = y;
}, 5000);
