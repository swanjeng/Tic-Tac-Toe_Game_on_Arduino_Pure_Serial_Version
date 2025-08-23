#define AI 1
#define HUMAN 2

byte board[3][3] = {{0, 0, 0},
                    {0, 0, 0},
                    {0, 0, 0}};

byte whosTurn;
bool firstRun = 1;

bool valid(String input, String c) {
  if (c == "yn") {
    if (input == "yes" || input == "no") return 1;
    else return 0;
  } else if (c == "place") {
    byte p = input.toInt();
    if (p < 1 || p > 9) return 0;
    if (board[2-(p-1)/3][(p-1)%3]) return 0;
    else return 1;
  }
}

void showBoard() {
  Serial.println("+-+-+-+");
  for (int i = 0 ; i < 3 ; i ++) {
    Serial.print("|");
    for (int j = 0 ; j < 3 ; j ++) {
      if (board[i][j] == 0) Serial.print(10 - (3 * i + (3 - j)));
      else if (board[i][j] == 1) Serial.print("O");
      else Serial.print("X");
      Serial.print("|");
    }
    Serial.println("\n+-+-+-+");
  }
}

bool full() {
  for (int i = 0 ; i < 3 ; i ++) {
    for (int j = 0 ; j < 3 ; j ++) {
      if (board[i][j] == 0) return 0;
    }
  }
  return 1;
}

bool checkWin(int player) {
  bool p1 = (board[0][0] == player && board[0][0] == board[0][1] && board[0][1] == board[0][2]);
  bool p2 = (board[1][0] == player && board[1][0] == board[1][1] && board[1][1] == board[1][2]);
  bool p3 = (board[2][0] == player && board[2][0] == board[2][1] && board[2][1] == board[2][2]);

  bool p4 = (board[0][0] == player && board[0][0] == board[1][0] && board[1][0] == board[2][0]);
  bool p5 = (board[0][1] == player && board[0][1] == board[1][1] && board[1][1] == board[2][1]);
  bool p6 = (board[0][2] == player && board[0][2] == board[1][2] && board[1][2] == board[2][2]);

  bool p7 = (board[0][0] == player && board[0][0] == board[1][1] && board[1][1] == board[2][2]);
  bool p8 = (board[0][2] == player && board[0][2] == board[1][1] && board[1][1] == board[2][0]);

  if (p1 || p2 || p3 || p4 || p5 || p6 || p7 || p8) return 1;
  else return 0;
}

void getAIMove() {
  for (int i = 1 ; i <= 9 ; i ++) {
    if (!valid(String(i), "place")) continue;
    board[2-(i-1)/3][(i-1)%3] = AI;
    if (checkWin(AI)) {
      Serial.println("I choose " + String(i) + "!");
      return;
    }
    board[2-(i-1)/3][(i-1)%3] = 0;
  }
  for (int i = 1 ; i <= 9 ; i ++) {
    if (!valid(String(i), "place")) continue;
    board[2-(i-1)/3][(i-1)%3] = HUMAN;
    if (checkWin(HUMAN)) {
      board[2-(i-1)/3][(i-1)%3] = AI;
      Serial.println("I choose " + String(i) + "!");
      return;
    }
    board[2-(i-1)/3][(i-1)%3] = 0;
  }
  byte order[9] = {7, 9, 1, 3, 8, 4, 6, 2, 5};
  for (int i = 0 ; i < 9 ; i ++) {
    int x = order[i];
    if (valid(String(x), "place")) {
      board[2-(x-1)/3][(x-1)%3] = AI;
      Serial.println("I choose " + String(order[i]) + "!");
      return;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (firstRun) {
    Serial.println(F("Hello~ Let's play Tic-Tac-Toe~"));
    firstRun = 0;
  }
  for (int i = 0 ; i < 3 ; i ++) {
    for (int j = 0 ; j < 3 ; j ++) {
      board[i][j] = 0;
    }
  }
  Serial.println("I am 'O' and you are 'X'.");
  String input = "";
  do {
    Serial.println(F("Do you want to be the first? Yes or No"));
    while (!Serial.available());
    input = Serial.readStringUntil('\n');
    input.trim();
    Serial.println("You: " + input);
    input.toLowerCase();
    if (!valid(input, "yn")) Serial.println("invalid...");
  } while (!valid(input, "yn"));

  if (input == "yes") whosTurn = HUMAN;
  else whosTurn = AI;

  while (!full() && !checkWin(AI) && !checkWin(HUMAN)) {
    showBoard();
    if (whosTurn == HUMAN) {
      input = "";
      do {
        Serial.println(F("Enter an integer between 1 and 9"));
        Serial.println(F("1 refers to the bottom left, and 9 refers to the top right."));
        while (!Serial.available());
        input = Serial.readStringUntil('\n');
        input.trim();
        Serial.println("You: " + input);
        if (!valid(input, "place")) Serial.println("invalid...");
      } while (!valid(input, "place"));
      int p = input.toInt();
      board[2-(p-1)/3][(p-1)%3] = whosTurn;
      whosTurn = 3 - whosTurn;
    } else {
      Serial.println("Hmm...");
      getAIMove();
      whosTurn = 3 - whosTurn;
    }
  }
  showBoard();
  if (checkWin(AI)) Serial.println("I win!");
  else if (checkWin(HUMAN)) Serial.println("You win!");
  else Serial.println("Tie!");
  
  input = "";
  do {
    Serial.println(F("Wanna play again? Yes or No"));
    while (!Serial.available());
    input = Serial.readStringUntil('\n');
    input.trim();
    Serial.println("You: " + input);
    input.toLowerCase();
    if (!valid(input, "yn")) Serial.println("invalid...");
  } while (!valid(input, "yn"));
  if (input == "no") while (1);
}
