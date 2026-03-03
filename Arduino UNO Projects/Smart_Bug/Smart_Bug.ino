#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// ===== WiFi =====
const char* ssid = "RobotArm";
const char* password = "12345678";

// ===== L298N Pins =====
#define ENA 26
#define IN1 13
#define IN2 14
#define ENB 25
#define IN3 27
#define IN4 33

// ===== Servo Pins =====
#define SHOULDER_PIN 15
#define ELBOW_PIN 16
#define PINCHER_PIN 17

// ===== Ultrasonic =====
#define TRIG_PIN 4
#define ECHO_PIN 5

// ===== Buzzer =====
#define BUZZER_PIN 2

// ===== PWM =====
#define PWM_FREQ 1000
#define PWM_RES 8
#define CH_ENA 0
#define CH_ENB 1

WebServer server(80);
Servo shoulder, elbow, pincher;

// ===== Globals =====
int speedFactor = 200;
int shoulderAngle = 90;
int elbowAngle = 90;
int pincherAngle = 45;

// ================== WEB PAGE ==================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Robot Controller</title>
<style>
body{font-family:Arial;text-align:center;background:#f2f2f2;margin:0}
h2{background:#2196F3;color:white;padding:12px;margin:0}
.btn{
  width:90px;height:90px;font-size:28px;margin:8px;
  border:none;border-radius:15px;background:#4CAF50;color:white;
}
.stop{background:#f44336}
.slider{width:85%}
.card{
  background:white;margin:15px;padding:15px;border-radius:12px;
  box-shadow:0 0 10px #ccc;
}
</style>
</head>
<body>

<h2>🤖 Robot Arm Controller</h2>

<div class="card">
<h3>🚗 Car Control</h3>
<button class="btn" onclick="move('F')">⬆️</button><br>
<button class="btn" onclick="move('L')">⬅️</button>
<button class="btn stop" onclick="move('S')">⏹️</button>
<button class="btn" onclick="move('R')">➡️</button><br>
<button class="btn" onclick="move('B')">⬇️</button>
</div>

<div class="card">
<h3>⚡ Speed</h3>
<input type="range" class="slider" min="100" max="255" value="200"
oninput="setSpeed(this.value)">
</div>

<div class="card">
<h3>🦾 Arm Control</h3>

Shoulder: <span id="s1">90</span>°<br>
<input class="slider" type="range" min="0" max="180" value="90"
oninput="servo('shoulder',this.value,'s1')"><br>

Elbow: <span id="s2">90</span>°<br>
<input class="slider" type="range" min="0" max="180" value="90"
oninput="servo('elbow',this.value,'s2')"><br>

Pincher: <span id="s3">45</span>°<br>
<input class="slider" type="range" min="30" max="120" value="45"
oninput="servo('pincher',this.value,'s3')">
</div>

<div class="card">
<h3>📏 Distance: <span id="dist">0</span> cm</h3>
<button class="btn" onclick="beep()">🔊</button>
</div>

<script>
function move(dir){ fetch('/move?dir='+dir); }
function setSpeed(v){ fetch('/speed?val='+v); }
function servo(name,val,id){
 document.getElementById(id).innerHTML=val;
 fetch('/'+name+'?angle='+val);
}
function beep(){ fetch('/buzzer'); }

setInterval(()=>{
 fetch('/distance').then(r=>r.text()).then(t=>{
   document.getElementById('dist').innerHTML=t;
 });
},700);
</script>

</body>
</html>
)rawliteral";

// ================== SETUP ==================
void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // PWM
  ledcSetup(CH_ENA, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENA, CH_ENA);
  ledcSetup(CH_ENB, PWM_FREQ, PWM_RES);
  ledcAttachPin(ENB, CH_ENB);

  // Servos
  shoulder.attach(SHOULDER_PIN);
  elbow.attach(ELBOW_PIN);
  pincher.attach(PINCHER_PIN);

  // WiFi AP
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());

  // Routes
  server.on("/", [](){ server.send(200,"text/html",index_html); });
  server.on("/move", handleMove);
  server.on("/speed", [](){ speedFactor = server.arg("val").toInt(); server.send(200,"text/plain","OK"); });
  server.on("/shoulder", [](){ shoulder.write(server.arg("angle").toInt()); server.send(200,"text/plain","OK"); });
  server.on("/elbow", [](){ elbow.write(server.arg("angle").toInt()); server.send(200,"text/plain","OK"); });
  server.on("/pincher", [](){ pincher.write(server.arg("angle").toInt()); server.send(200,"text/plain","OK"); });
  server.on("/buzzer", [](){ digitalWrite(BUZZER_PIN,!digitalRead(BUZZER_PIN)); server.send(200,"text/plain","OK"); });
  server.on("/distance", handleDistance);

  server.begin();
}

// ================== LOOP ==================
void loop() {
  server.handleClient();
}

// ================== MOVE ==================
void handleMove(){
  String d = server.arg("dir");

  if(d=="F"){ motor(speedFactor, speedFactor); }
  else if(d=="B"){ motor(-speedFactor, -speedFactor); }
  else if(d=="L"){ motor(-speedFactor, speedFactor); }
  else if(d=="R"){ motor(speedFactor, -speedFactor); }
  else motor(0,0);

  server.send(200,"text/plain","OK");
}

// ================== MOTOR ==================
void motor(int left,int right){
  setMotor(IN1,IN2,CH_ENA,left);
  setMotor(IN3,IN4,CH_ENB,right);
}

void setMotor(int in1,int in2,int ch,int sp){
  if(sp>0){
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    ledcWrite(ch,sp);
  } else if(sp<0){
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    ledcWrite(ch,-sp);
  } else {
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
    ledcWrite(ch,0);
  }
}

// ================== DISTANCE ==================
void handleDistance(){
  digitalWrite(TRIG_PIN,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN,LOW);

  long dur=pulseIn(ECHO_PIN,HIGH,30000);
  long dist=dur*0.034/2;
  server.send(200,"text/plain",String(dist));
}
