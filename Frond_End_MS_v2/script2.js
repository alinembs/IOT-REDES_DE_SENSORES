
const themeSwitch = document.getElementById("btn_mode");
if (themeSwitch) {
    themeSwitch.addEventListener('click', () => {
        console.log('btn clicked')

    })


};

const body = document.getElementById("body");
const manual = document.getElementById("manual-m");

themeSwitch.addEventListener("click", () => {


    if (body.className === "eyes-off") {
        manual.style.display = "grid";
        body.className = "eyes-on";
    } else if (body.className === "eyes-on") {
        manual.style.display = "none";
        body.className = "eyes-off";

    }
});


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

function rangeSlide(value) {
    document.getElementById('rangeValue').innerHTML = value + "cm";
}

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

var classname = document.getElementsByClassName("button");

for (var i = 0; i < classname.length; i++) {
    classname[i].addEventListener('click', animateButton, false);
}
