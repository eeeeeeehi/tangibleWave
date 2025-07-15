#pragma once
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxNetwork.h"
#include "ofxLibRealSense2.hpp"
#include <vector>
#define IP "192.168.110.199"
#define PORT 11999
#define MacNum 116
#define mainHeight 720
#define subHeight 360
#define width 1280
#define height 1080

// 検出された円を表す構造体
struct myCircle {
   ofVec2f center;
   float radius;
    float wavelength;
};
struct Wave{
    ofVec2f center;
    float radius;
    float maxRadius;
    float wavelength;
    float frequency;
    float amplitude;
};
class ofApp : public ofBaseApp {
public:
   void setup();
   void update();
    void drawMain();
    void drawSub();
   void draw();
   void keyPressed(int key);
   void keyReleased(int key);
   void exit();
   void mesSend(string QRNumber, string mes);
   void sendText2server( string txt );
    
    //文字
    ofTrueTypeFont font;    
    //元のウィンドウサイズを保持
    int originalWidth,originalHeight;
    //現在のウィンドウサイズの比率
    float screenRatioX,screenRatioY;
    
    ofFbo mainFbo;
    ofFbo subFbo;
    float time;
    
    //ゲーム
    void checkConditions();
    void advanceToNextStage();
    
    bool checkWaveTouchingInnerLines();
    
    
    int currentStage;
    float conditionMetDuration,conditionMetDuration2,conditionMetDuration3,maxConditionMetTime;
    
    
private:
    void writeLog(string log );
    void writeLog2( string log );
   void updateDepth();
   void updateCircles();
    void updateTimer();
    void updateWave();
    void updateRankings(int score);
   void drawSelect();
   void drawSurvey();
    void drawWave();
    void drawWave2();
    void drawScene();
    void drawMessage();
    void drawTimer();
    void drawComment();
    void drawRankings();
    void drawSlider();
    void drawQuiz();
   
    
    int calculatePoints(int stage, int interval);
   
    
   ofxPanel gui; // GUI
    ofxFloatSlider roundness;
    ofxIntSlider    depthMin;
    ofxIntSlider    depthMax;
    ofxFloatSlider wavelength;
    ofxFloatSlider frequency;
    ofxFloatSlider velocity;
    ofxFloatSlider dotSize;
    ofxFloatSlider minRadius;
    ofxFloatSlider maxRadius;
    
   ofxLibRealSense2 _realsense2; // RealSense2オブジェクト
   uint8_t* depthWork; // 深度データを格納するバッファ
   ofTexture workTex; // 深度データを描画するためのテクスチャ
   ofxCvGrayscaleImage grayImage; // グレースケール画像
   ofxCvContourFinder cf; // 輪郭検出
   vector<ofPolyline> edgeLines; // 検出された輪郭線
   vector<myCircle> theCircles; // 検出された円
   vector<myCircle> currentCircles;
   vector<myCircle> previousCircles;
   vector<Wave> waves;
    
    //画像
    ofImage iButton_Start,iButton_Retry,iButton_Quit,iButton_Quit2,iButton_Play,iButton_Stop,iButton_Hint,iButton_Setumei,image[20],iButton[10],quizImage[10],kaisetsuImage[10];
    
    //音
    ofSoundPlayer sound[20];
    
    //動画
    ofVideoPlayer video[5];
    int video_start[5];
    
    //display mode
    bool isDrawEnabled;
    
    ofMesh waveMesh;
    int gridSize = 7;
    float squareSize = gridSize ;//正方形のサイズを調整
    
    //ゲーム
    float timer,timer2;
    float maxTimer,maxTimer2;
    bool timerVisible;
    bool isGameMode; //ゲームモードにいるかどうかを追跡するフラグ
    bool isPaused = false; //波停止
    
    //ステージ条件に基づく文字色
    ofColor messageColor;
    
    bool isConditionMet;
    bool isWaveTouchingInnerLines = false; //波が内側の下線に触れるかどうかのフラグ
    bool isWaveTouchingOuterLines = false; //波が外側の下線に触れるかどうかのフラグ
    
    int scene;
    
    float lastWaveTime = 0.0f;
    float waveInterval = 0.5f; //波源を追加する間隔（秒）
    
    float minDistanceBetweenWaves = 50.0f;//波源間の最小距離

    //キオスク関連
    ofxTCPClient tcpClient; 
    int    QR_1 = -1,QR_2 = 0;
    string UserNum;
    
    //ゲームタイマー
    float gameTimer;
    bool gameStarted;
    bool gameFinished;
    
    //スタートボタン
    int buttonWidth[10],buttonHeight[10],buttonX[10],buttonY[10];
    float dist,dist2;
   
    bool isFullscreen = false; //フルスクリーン状態を追跡
    
    //drawside
    int outer,inner;
    
    //updateSelect
    int u,Area_x[5], Area_y[5], Area_w[5], Area_h[5];
    
    //アンケート
    int questionCount,ratingCount;
    std::vector<int> responses; //回答を格納するベクター
    //アンケート表示のフラグ
    bool isSurveyActive = false;
    
    ofFile logFile;
    
    // スクリーンショット保存用
    ofImage ScreenShot;
    int screenshotCount;
    
    bool isInRestrictedArea;
    
    int interval = 0;
    const int maxInterval = 20;
    const int minInterval = 0;
    
    float levelTimer = 0.0f;
    
    float baseY = subHeight / 2;
    float wavePositionY;
    
    int totalPoints = 0;
    std::vector<int> rankings;
    
    //クリア
    bool clear = false;
    
    int currentScene = 0; //現在のサブシーン
    unsigned long lastSceneChangeTime = 0;//最後のシーン変更時間
    
    //スライダーの定義
    float sliderWidth = 1150;
    float sliderWidth2 = 200;
    float sliderHeight = 50;
    float sliderX = 100;
    float sliderX2 = width / 2;
    float wavelengthSliderY = height - 160 - 360;
    float frequencySliderY = height - 150 - 360;
    float velocitySliderY = height - 70 - 360;
    float minSliderX = sliderX;//スライダーの最小X位置
    float minSliderX2 = sliderX2;
    float maxSliderX = sliderX + sliderWidth;
    float maxSliderX2 = sliderX2 + sliderWidth2;
    
    int minWavelength = 50;
    int maxWavelength = 200;
    
    //現在の円の位置調整量
    float offsetX = 0;
    float offsetY = 0;
    
    bool hasPlayedSound8 = false;
    bool hasPlayedSound9 = false;
    bool hasPlayedSound10 = false;
    
    int frameCount = 0;

    int selectedContent;
    
    float shadowOffset = 20;
    
    //quiz
    std::vector<int> quizResponses; //回答を格納するベクター
    int answerCount;
    bool showAnswer;
    float sleepTime;
    bool showKaisetsu1,showKaisetsu2,showKaisetsu3,showHint;
    
    //プレイ時間
    float startPlayTime; //開始時刻
    bool contentPlay = false;
    
    //ログ
    bool isLogTaken = false;//プレイ時間
    bool logWritten = false;//アンケート
    
    //音の再生時間（ボタン）
    float soundStartTime;
    float playDuration = 1.5f;
    
    bool waveExists;
    bool foundMacthingCircle;
    
    //共通の描画サイズ
    float targetHeight = 100.0;
    ofImage selectedImage;
    bool volumeDown = false;
};
