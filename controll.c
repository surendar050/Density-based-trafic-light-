// Lane Pins: {Red, Yellow, Green}
int laneA[] = {2, 3, 4};
int laneB[] = {5, 6, 7};
int laneC[] = {8, 9, 10};
int laneD[] = {11, 12, 13};

// Global vehicle counts received from Python
int a = 0, b = 0, c = 0, d = 0;

// Timing constants
const int base_green = 5000;    // Minimum 5 seconds green
const int sec_per_car = 2000;   // Add 2 seconds per detected vehicle
const int max_green = 30000;    // Maximum 30 seconds green
const int yellow_time = 2000;   // 2 seconds yellow transition

void setup() {
  Serial.begin(9600);
  
  // Set all pins as output
  for(int i=2; i<=13; i++) pinMode(i, OUTPUT);
  
  // Start with all Reds ON
  allRed();
}

void loop() {
  // 1. Check for fresh data from Python
  updateVehicleCounts();

  // 2. Cycle through each lane
  processLane("Lane A", laneA, a);
  processLane("Lane B", laneB, b);
  processLane("Lane C", laneC, c);
  processLane("Lane D", laneD, d);
}

void processLane(String name, int pins[], int vehicleCount) {
  // Calculate dynamic green time: T = base + (count * factor)
  int greenDuration = base_green + (vehicleCount * sec_per_car);
  if (greenDuration > max_green) greenDuration = max_green;

  // Green Phase
  digitalWrite(pins[0], LOW);  // Red OFF
  digitalWrite(pins[2], HIGH); // Green ON
  delay(greenDuration);

  // Yellow Phase
  digitalWrite(pins[2], LOW);  // Green OFF
  digitalWrite(pins[1], HIGH); // Yellow ON
  delay(yellow_time);

  // Red Phase
  digitalWrite(pins[1], LOW);  // Yellow OFF
  digitalWrite(pins[0], HIGH); // Red ON
}

void updateVehicleCounts() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int firstComma = data.indexOf(',');
    int secondComma = data.indexOf(',', firstComma + 1);
    int thirdComma = data.indexOf(',', secondComma + 1);

    if (firstComma != -1 && secondComma != -1 && thirdComma != -1) {
      a = data.substring(0, firstComma).toInt();
      b = data.substring(firstComma + 1, secondComma).toInt();
      c = data.substring(secondComma + 1, thirdComma).toInt();
      d = data.substring(thirdComma + 1).toInt();
    }
  }
}

void allRed() {
  digitalWrite(2, HIGH); digitalWrite(5, HIGH);
  digitalWrite(8, HIGH); digitalWrite(11, HIGH);
}