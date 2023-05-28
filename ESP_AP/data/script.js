function logoutButton() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/logout", true);
    xhr.send();
    setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
  }
  function controlOutput(element) {
    var xhr = new XMLHttpRequest();
    if(element.checked){ xhr.open("GET", "/output?state=1", true); }
    else { xhr.open("GET", "/output?state=0", true); }
    xhr.send();
  }
  function toggleLed(element) {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/toggle", true);
    xhr.send();
  }
  function clearMotionAlert() {
    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/clear-motion", true);
    xhr.send();
    setTimeout(function(){
      document.getElementById("motion").innerHTML = "No motion";
      document.getElementById("motion").style.color = "#3b3b3b";
    }, 1000);
  }
  if (!!window.EventSource) {
   var source = new EventSource('/events');
   source.addEventListener('open', function(e) {
    console.log("Events Connected");
   }, false);
   source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
   }, false);
   source.addEventListener('message', function(e) {
    console.log("message", e.data);
   }, false);
   source.addEventListener('led_state', function(e) {
    console.log("led_state", e.data);
    var inputChecked;
    if( e.data == 1){ inputChecked = true; }
    else { inputChecked = false; }
    document.getElementById("led").checked = inputChecked;
   }, false);
   source.addEventListener('motion', function(e) {
    console.log("motion", e.data);
    document.getElementById("motion").innerHTML = e.data;
    document.getElementById("motion").style.color = "#b30000";
   }, false); 
   source.addEventListener('temperature', function(e) {
    console.log("temperature", e.data);
    document.getElementById("temp").innerHTML = e.data;
   }, false);
   source.addEventListener('humidity', function(e) {
    console.log("humidity", e.data);
    document.getElementById("humi").innerHTML = e.data;
   }, false);
   source.addEventListener('light', function(e) {
    console.log("light", e.data);
    document.getElementById("light").innerHTML = e.data;
   }, false);
}