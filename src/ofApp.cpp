#include "ofApp.h"
 
//--------------------------------------------------------------
void ofApp::setup(){
    originalWidth = ofGetWidth();
    originalHeight = ofGetHeight();

    ofSetFrameRate(60);
    ofSetWindowShape(width,height);
   ofBackground(255);
    
    //sub
    ofSetWindowPosition(0,0);
    //main
    ofSetWindowPosition(0,subHeight);
    
    //FBOの初期化
    mainFbo.allocate(width,mainHeight,GL_RGBA);
    subFbo.allocate(width,subHeight,GL_RGBA);
    
    mainFbo.begin();
    ofClear(0,0,0,0);
    mainFbo.end();
    
    subFbo.begin();
    ofClear(0,0,0,0);
    subFbo.end();
    
    time = 0.0;
    
   // GUI設定
   gui.setup();
    gui.setPosition(400,360);
   gui.add(roundness.setup("Roundness", 0.25, 0.0, 1.0));
   gui.add(depthMax.setup("Max Depth", 86, 30, 150));
   gui.add(depthMin.setup("Min Depth", 50, 10, 150));
    
    gui.add(wavelength.setup("Wavelength",100,minWavelength,maxWavelength));
    gui.add(velocity.setup("velocity",5.0,5.0,6.0));
    gui.add(minRadius.setup("minRadius",200,100,500));
    gui.add(maxRadius.setup("maxRadius",31,30,90));
    
   // RealSenseの初期化
   _realsense2.setupDevice(0);
   _realsense2.setupColor(width,mainHeight,30);
   _realsense2.setupIR(width,mainHeight,30);
   _realsense2.setupDepth(width,mainHeight,30);
   _realsense2.startPipeline(true);
   depthWork = new uint8_t[_realsense2.getDepthWidth() * _realsense2.getDepthHeight()];
   workTex.allocate(_realsense2.getDepthWidth(), _realsense2.getDepthHeight(), GL_LUMINANCE);
   grayImage.allocate(width,mainHeight);
    
    waves.clear();
    
    
    //文字
    //  for Japanese Text
    ofTrueTypeFontSettings settings("Arial.ttf", 30);
    settings.addRanges(ofAlphabet::Latin);
    settings.addRanges(ofAlphabet::Japanese);
    font.load(settings);
    
    //画像
    iButton_Start.load("start.png");
    iButton[0].load("gameButton.png");
//    iButton[1].load("kannsatsubutton.png");
    iButton[1].load("startButton.png");
    iButton[2].load("kettei.png");
    iButton_Retry.load("iButton_Retry.png");
    iButton_Quit.load("quit.png");
    iButton_Quit2.load("yameru.png");
    iButton_Play.load("play.png");
    iButton_Stop.load("stop.png");
    iButton_Hint.load("hint.png");
    iButton_Setumei.load("setumeiButton.png");
    image[0].load("setumei.png");
    image[1].load("questionnaire.png");
    image[2].load("syuuhasuu.png");
    image[3].load("hatyou.png");
    image[4].load("hint_nami2.png");
    image[5].load("hayasa.png");
    image[6].load("maru.png");
    image[7].load("batsu.png");
    image[8].load("5wavelength.png");
    image[9].load("4wavelength.png");
    image[10].load("3wavelength.png");
    image[11].load("2wavelength.png");
    image[12].load("1wavelength.png");
    quizImage[0].load("newQuiz1.png");
    quizImage[1].load("newQuiz2.png");
    quizImage[2].load("newQuiz3.png");
    quizImage[3].load("newQuiz4.png");
    kaisetsuImage[0].load("newQuiz1Answer.png");
    kaisetsuImage[1].load("newQuiz2Answer.png");
    kaisetsuImage[2].load("newQuiz3Answer.png");
    kaisetsuImage[3].load("newQuiz4Answer.png");
    
    //音
    //sound
    sound[0].load("sCheck.mp3");
    sound[1].load("sSelect.mp3");
    sound[2].load("sSelecting.mp3");
    sound[3].load("sSucceed.mp3");
    sound[4].load("sBgm.mp3");
    sound[5].load("sGamebgm.mp3");
    sound[6].load("sWater.mp3");
    sound[7].load("Countdown01-3.mp3");
    sound[8].load("sound1.mp3");
    sound[9].load("sound2.mp3");
    sound[10].load("sound3.mp3");
//    sound[11].load("rhythm.mp3");
    sound[11].load("do1.mp3");
    sound[12].load("false.mp3");
    
    //動画
    video[0].load("setumei1.mp4");
    video[0].setLoopState(OF_LOOP_NORMAL);
    video[0].play();
    video_start[0]=0;
    video[1].load("setumei2.mp4");
    video[1].setLoopState(OF_LOOP_NORMAL);
    video[1].play();
    video_start[1]=0;
    
    currentStage = 0; //ステージ0からスタート
    conditionMetDuration = 0.0f; //条件が満たされた時間を追跡するタイマー(ゲーム用)
    conditionMetDuration2 = 0.0f;//条件が満たされた時間を追跡するタイマー(スタートボタン用)
    conditionMetDuration3 = 0.0f;//条件が満たされた時間を追跡するタイマー(ヒント用)
    maxConditionMetTime = 5.0f; //条件が5秒間満たされたら次のステージへ進む
    
    timer = 0.0f; //タイマーの初期化
    timer2 = 0.0f;
    maxTimer = 5.0f; //ゲーム用
    maxTimer2 = 5.0f; //観察用
    timerVisible = false; //タイマーは初期状態で非表示
    
    //場面
    scene = 0;
    selectedContent = 0;
    
    //ゲームタイマーの追加
    gameTimer = 0.0f;
    gameStarted = false;
    gameFinished = false;
    
    //GUIとRealSense消す
    isDrawEnabled = true;
    
    
    //ボタン情報
    buttonWidth[0] = 90;//スタートボタンとか
    buttonHeight[0] = 70;
    buttonWidth[1] = 60; //再生停止ボタン用
    buttonHeight[1] = 60;
    buttonWidth[2] = width/5; //コンテンツ選択
    buttonHeight[2] = width/5;
    
     buttonX[0] = ofGetWindowWidth() / 2 - 30;//スタートボタン
     buttonY[0] = ofGetWindowHeight() - 250 - 360;
    buttonX[1] = ofGetWindowWidth() / 2;//再生停止ボタン用
    buttonY[1] = ofGetWindowHeight() - 60 - 360;
    buttonX[3] = 200;//ヒント
    buttonY[3] = ofGetWindowHeight() - 70 - 360;
    buttonX[4] = ofGetWindowWidth() - 100;//右上
    buttonY[4] = 80;
    buttonX[2] = ofGetWindowWidth() - 100;//アンケートあり(右中央)
    buttonY[2] = mainHeight / 2;
    //コンテンツ選択
//    buttonX[5] = width/5;
//    buttonY[5] = mainHeight / 2;
//    buttonX[6] = (3*width)/5;
//    buttonY[6] = mainHeight / 2;
    
    buttonX[6] = (width / 2) - ( buttonWidth[2]/2 );
    buttonY[6] = mainHeight / 2;
    
    
    //アンケート
    questionCount = 3; //質問の数
    ratingCount = 5; //評価の段階
    responses.resize(questionCount);//回答のベクターを初期化
    bool  allQuestionsAnswered = false;
    
    //quiz
    answerCount = 4;
    quizResponses.resize(answerCount);//回答のベクターを初期化
    showAnswer = false;
    sleepTime = 2.0f;
    
    //logフォルダの作成
    ofDirectory::createDirectory("log",true,true);
    
    //QR用
    ofSetEscapeQuitsApp(false);
    
    //スクショ用
    screenshotCount = 0;
    //フォルダの存在を確認し、なければ作成
    ofDirectory dir("screenshots");
    if(!dir.exists()){
        dir.create(true); //フォルダを作成
    }
    //フォルダ内のファイル数を数える
//    screenshotCount = dir.listDir(); //フォルダないのファイル数を取得
    
    
}
//--------------------------------------------------------------
void ofApp::writeLog(string log ){
    logFile.open( "log/"+ofGetTimestampString("%F")+".txt", ofFile::Append );
    logFile << ofGetTimestampString("%F %T") + " " + log + "\n";
    logFile.close();
}
//--------------------------------------------------------------
void ofApp::writeLog2(string log ){
    logFile.open( "log/"+ofGetTimestampString("%F")+".csv", ofFile::Append );
    logFile << ofGetTimestampString("%F %T") + " " + log + "\n";
    logFile.close();
}
//--------------------------------------------------------------
void ofApp::updateDepth(){
   uint16_t* depthBuff = _realsense2.getRawDepthBuff();
   if (depthBuff != NULL) {
       int max = _realsense2.getDepthWidth() * _realsense2.getDepthHeight();
       for (int i = 0; i < max; i++) {
           depthWork[i] = (uint16_t)(depthBuff[i] / 10);
           if (depthMin < depthWork[i] && depthWork[i] < depthMax) {
               depthWork[i] = 255;
           } else {
               depthWork[i] = 0;
           }
       }
       workTex.loadData(depthWork, _realsense2.getDepthWidth(), _realsense2.getDepthHeight(), GL_LUMINANCE);
       ofPixels pix;
       workTex.readToPixels(pix);
       grayImage.setFromPixels(pix);
       cf.findContours(grayImage, (1280 * 720) / minRadius, (1280 * 720) / maxRadius, 10, false, true); // 輪郭線を得る
   }
   edgeLines.clear();
   for (int u = 0; u < cf.nBlobs; u++) {
       ofPolyline line;
       for (int m = 0; m < cf.blobs[u].pts.size(); m++) {
           line.addVertex(cf.blobs[u].pts[m]);
       }
       edgeLines.push_back(line);
   }
}
//--------------------------------------------------------------
void ofApp::updateCircles(){
   theCircles.clear();
   for (int u = 0; u < cf.nBlobs; u++) {
       double round = 4.0 * M_PI * cf.blobs[u].area / (cf.blobs[u].length * cf.blobs[u].length);
       
       
       if (roundness < round ) {
           myCircle circ;
           circ.center = cf.blobs[u].centroid;
           circ.radius = (2.0 * cf.blobs[u].area / cf.blobs[u].length) * 1.1;
           //キー入力による位置調整
           circ.center.x += offsetX;
           circ.center.y += offsetY;
           
           //大きい円と小さい円で波長を分ける(maxdepth=89)
           
           if(circ.radius > 60){
               circ.wavelength = 300;
           }else if(circ.radius <= 60 && circ.radius > 40){
               circ.wavelength = 150;
           }else{
               circ.wavelength = 60;
           }
           
           theCircles.push_back(circ); //円を登録
           
           if(scene == 2){
           //波長スライダーに対応
        if(selectedContent == 2){
           if(circ.center.y > wavelengthSliderY && circ.center.y < wavelengthSliderY + sliderHeight && circ.center.x > minSliderX && circ.center.x < maxSliderX){
               wavelength = ofMap(circ.center.x,minSliderX,maxSliderX,minWavelength,maxWavelength,true);
                }
            
            //画面下に物体が入ると波をリセットする．（観察）
                if(circ.center.y > wavelengthSliderY){
//                    waves.clear();
                }
               }
               //速度スライダーに対応
               if(circ.center.y > velocitySliderY && circ.center.y < velocitySliderY + sliderHeight && circ.center.x > minSliderX2 && circ.center.x < maxSliderX2){
                   velocity = ofMap(circ.center.x,minSliderX2,maxSliderX2,2,6,true);
               }
           }
       }
   }
}
//--------------------------------------------------------------
void ofApp::updateTimer(){

    //スタートボタンの状態を管理
    bool overlapping = false;
    bool overlapping2 = false;
    
    //スタートボタン
    //距離を計算
    for(const auto& circ : theCircles){
        if(scene == 0){
            dist = ofDist(buttonX[5]+buttonWidth[2]/2,buttonY[5]+buttonHeight[2]/2, circ.center.x,circ.center.y);
            dist2 = ofDist(buttonX[6]+buttonWidth[2]/2,buttonY[6]+buttonHeight[2]/2, circ.center.x,circ.center.y);
            
            
        }else if(scene == 2){
            dist = ofDist(buttonX[1] + 50 ,buttonY[1] + 40, circ.center.x,circ.center.y);
            dist2 = ofDist(buttonX[4] + 50 ,buttonY[4] + 40, circ.center.x,circ.center.y);
        }else if(scene == 4){
            dist = ofDist(buttonX[2] + 50 ,buttonY[2] + 40, circ.center.x,circ.center.y);
        }
            
            //円とボタンが重なっているか確認
                if(dist < (50 + circ.radius)){//もと50
                    overlapping = true;
                    break;
                }
                if(dist2 < (50 + circ.radius)){
                        overlapping2 = true;
                        break;
                }
        }
    
    if(overlapping || overlapping2){
    if(!sound[2].isPlaying()){
        sound[2].play();
    }
   
    //タイマーの進行と判定
    if(overlapping){
   
        //重なっている場合はタイマーを進行
            conditionMetDuration2 += ofGetLastFrameTime(); //フレームごとの経過時間を加算
            if(conditionMetDuration2 >= 1.5f){
//                if(scene == 0 || scene == 4){
//                sound[1].play();
//                }
                if(scene == 0){
                sendText2server( "1start" ); //wikiserver用
                writeLog("_______________1start___________________" + ofToString(UserNum));
                writeLog2(",content1");
                selectedContent = 2;
                scene = 2;
                sound[1].play();
                }else if(scene == 4){
                    
                    //QRが読み込まれなかった時
                    if(UserNum != ""){
                    mesSend(UserNum, "End");
                    }
                    UserNum = "";
                    writeLog("_______________end___________________");
                    writeLog2(",_______________end________________,");
                    scene = 0;
                    if(sound[11].isPlaying()){
                    sound[11].stop();
                    }
                    currentStage = 0;
                    waves.clear();
                    sound[1].play();
                }

            conditionMetDuration2 = 0.0f; //タイマーをリセット
                    
        }
    }else{
        conditionMetDuration2 = 0.0f; //重なりが解除されたらタイマーをリセット
        
        
    }
    //同じシーンで複数のボタンがあるとき
    if(overlapping2){
        //重なっている場合はタイマーを進行
            conditionMetDuration3 += ofGetLastFrameTime(); //フレームごとの経過時間を加算
            if(conditionMetDuration3 >= 1.5f){
                
            if(scene == 0){
//                if(!sound[1].isPlaying()){
//                sound[1].play();
//                }
                sendText2server( "2start" ); //wikiserver用
                writeLog("_______________2start___________________"+ ofToString(UserNum));
                writeLog2(",content2");
                scene = 2;
                selectedContent = 2;
                sound[1].play();
            }else if(scene == 2){
//                scene = 3;
                scene = 4;
                if(sound[11].isPlaying()){
                sound[11].stop();
                }
                sound[1].play();
                contentPlay = false;
                
            }
                conditionMetDuration3 = 0.0f;
            }
    }else{
        conditionMetDuration3 = 0.0f; //重なりが解除されたらタイマーをリセット
        
        
    }
}else{
    if(sound[2].isPlaying()){
        sound[2].stop();
    }
    
    conditionMetDuration2 = 0.0f;
    conditionMetDuration3 = 0.0f;
    
}
    if(scene == 3&&!isLogTaken){
    //プレイ時間を計算
    float playTime = ofGetElapsedTimef() - startPlayTime; //秒単位での経過時間
    writeLog("playTime:" + ofToString(playTime));
        isLogTaken = true;
    }
}
//--------------------------------------------------------------
void ofApp::updateWave(){
    if(video_start[0]==0){
        video[0].update();
    }
    if(video_start[1]==0){
        video[1].update();
    }
   

    if(scene != 4 && !isPaused){
        float currentTime = ofGetElapsedTimef();
        
            if(gameStarted && !gameFinished){
            gameTimer += ofGetLastFrameTime(); //経過時間を加算
        }
        
 

    //波源の更新
        for(auto& circ : theCircles){
            waveExists = false;
            //波が発生しないエリア
            if(scene != 4){
                isInRestrictedArea =  circ.center.y > 600;
                
            }
            
           
            //既存の波源が存在するか確認し、既存の波源を更新
            for(auto& wave : waves){
                if(ofDist(wave.center.x,wave.center.y,circ.center.x,circ.center.y) < minDistanceBetweenWaves){
                    wave.center = circ.center; //既存の波源の位置を更新
                  
                    waveExists =  true;
                    
                    break;
                }
            }
           
            if(!waveExists&&!isInRestrictedArea){
                Wave newWave;
                newWave.center = circ.center;
                newWave.radius = 0;
                newWave.maxRadius = ofGetWidth();
                newWave.amplitude = 1.0f;
                if(selectedContent == 0 || selectedContent == 2){
//                newWave.wavelength = wavelength;
                }else{
                newWave.wavelength = circ.wavelength;
                }
                waves.push_back(newWave);

            }
            if(selectedContent == 0 || selectedContent == 2){//リアルタイム波長変更
                for(auto& wave : waves){
                    wave.wavelength = wavelength;
                }
            }
            
        }
        
    //検出されなくなった円に対応する波を削除
        for(auto it = waves.begin(); it != waves.end();){
            foundMacthingCircle = false;
            for(const auto& circ : theCircles){
                if(ofDist(it->center.x,it->center.y,circ.center.x,circ.center.y)<minDistanceBetweenWaves){
                    foundMacthingCircle = true;
                    break;
                }
            }
            if(!foundMacthingCircle){
                it = waves.erase(it); //波を削除
                
            }else{
                ++it;
            }
        }
    
    for(auto& wave : waves){
        wave.radius += velocity;
        wave.amplitude *= 1.0f;
        if(wave.radius > wave.maxRadius){
            wave.radius = wave.maxRadius;
        }
    }
        //drawsideviewの横線の幅（難易度調整）
        inner = 40 + interval * 5;
        
        //自動難易度調整(10s毎に横線の幅を調整)
        levelTimer += ofGetLastFrameTime(); //タイマーを更新
        if(levelTimer >= 8.0f){
            if(currentStage == 2 || currentStage == 3){
            if(interval < maxInterval){
                interval++;
            }
            }else if(currentStage == 1 || currentStage == 4){
                if(interval > minInterval){
                    interval--;
                }
            }
            levelTimer = 0.0f;
        }
        
        //古い波を削除
        waves.erase(std::remove_if(waves.begin(),waves.end(),[](Wave& wave){
            return wave.radius >= wave.maxRadius;
        }),waves.end());
        if(selectedContent == 1){
            checkConditions(); //条件をチェック
        }
    }
}
//--------------------------------------------------------------
void ofApp::updateRankings(int score){
    //新しいスコアをベクターに保存
    rankings.push_back(score);
    //スコアの降順にソート
    std::sort(rankings.begin(),rankings.end(),std::greater<int>());
    //上位５つに絞る
    if(rankings.size() > 5){
        rankings.resize(5);
    }
}
//--------------------------------------------------------------
void ofApp::update(){
    frameCount++;
    
   _realsense2.update();
   updateDepth();
   updateCircles(); // 円の検出と更新
   updateTimer();
    updateWave();
    
    time += ofGetLastFrameTime();
    
    subFbo.begin();
    ofClear(255,255,255,0);
    drawSub();
    subFbo.end();
    
    mainFbo.begin();
    ofClear(0,0,0,0);
    drawMain();
    mainFbo.end();
    
    //QR
    if (tcpClient.isConnected()) {
           
       }
    if (QR_1 == 0 && scene == 0) {
            mesSend(UserNum, "Start");
        
            QR_1 = -1;
//            scene = 1;
        }
  
}
//--------------------------------------------------------------
void ofApp::drawSelect(){
    
    if(scene == 0){
        //かげ
        ofSetColor(0,0,0,70);

//        ofDrawCircle(buttonX[5]+shadowOffset+buttonWidth[2]/2,buttonY[5]+shadowOffset+buttonHeight[2]/2,buttonWidth[2]/2);

        ofDrawCircle(buttonX[6]+shadowOffset+buttonWidth[2]/2,buttonY[6]+shadowOffset+buttonHeight[2]/2,buttonWidth[2]/2);
        
        ofSetColor(255);
//        iButton[0].draw(buttonX[5],buttonY[5],buttonWidth[2],buttonHeight[2]);
        iButton[1].draw(buttonX[6],buttonY[6],buttonWidth[2],buttonHeight[2]);

    //コンテンツ１用
    //ボタンの進行度に基づいて赤く染まるようにする
    float progress = ofMap(conditionMetDuration2, 0, 1.5f, 0, buttonWidth[2] / 2, true);
    ofSetColor(255,0,0,150); //赤
        ofEnableAlphaBlending();
        ofFill();
 //赤くなる部分を描画
//        ofDrawCircle(buttonX[5]+buttonWidth[2] / 2,buttonY[5]+buttonHeight[2] / 2,progress);

        //ボタン画像の形状に合わせて染めるために、アルファチャンネルでマスク
        iButton[0].getTexture().setAlphaMask(iButton[0].getTexture());//ボタンの形に合わせる

        ofDisableAlphaBlending();
        
        //コンテンツ２用
        //ボタンの進行度に基づいて赤く染まるようにする
        float progress2 = ofMap(conditionMetDuration3, 0, 1.5f, 0, buttonWidth[2] / 2, true);
        ofSetColor(255,0,0,150); //赤
            ofEnableAlphaBlending();
            ofFill();
 //赤くなる部分を描画
        ofDrawCircle(buttonX[6]+buttonWidth[2] / 2,buttonY[6]+buttonHeight[2] / 2,progress2);

            //ボタン画像の形状に合わせて染めるために、アルファチャンネルでマスク
            iButton[1].getTexture().setAlphaMask(iButton[1].getTexture());//ボタンの形に合わせる

            ofDisableAlphaBlending();
        
        
    }else if(scene == 4){
        ofSetColor(255);
        iButton[2].draw(buttonX[2],buttonY[2],buttonWidth[0],buttonHeight[0]);

    //ボタンの進行度に基づいて赤く染まるようにする
    float progress = ofMap(conditionMetDuration2, 0, 1.5f, 0, buttonWidth[0], true);
    ofSetColor(255,0,0,150); //赤
        ofEnableAlphaBlending();
        ofFill();
        ofDrawRectangle(buttonX[2],buttonY[2],progress,buttonHeight[0]); //赤くなる部分を描画

        //ボタン画像の形状に合わせて染めるために、アルファチャンネルでマスク
        iButton_Quit2.getTexture().setAlphaMask(iButton_Quit2.getTexture());//ボタンの形に合わせる

        ofDisableAlphaBlending();
        
        
    }else if(scene == 2){
        ofSetColor(255);

        iButton_Quit2.draw(buttonX[4],buttonY[4],buttonWidth[0],buttonHeight[0]);
        
        //ボタンの進行度に基づいて赤く染まるようにする
        float progress = ofMap(conditionMetDuration3, 0, 1.5f, 0, buttonWidth[0], true);
        ofSetColor(255,0,0,150); //赤
            ofEnableAlphaBlending();
            ofFill();
            ofDrawRectangle(buttonX[4],buttonY[4],progress,buttonHeight[0]); //赤くなる部分を描画

            //ボタン画像の形状に合わせて染めるために、アルファチャンネルでマスク
            iButton_Quit2.getTexture().setAlphaMask(iButton_Quit2.getTexture());//ボタンの形に合わせる

            ofDisableAlphaBlending();
    }
    
    // 検出された円の描画
   
    ofSetLineWidth(0.5);
    ofSetColor(255, 255, 255);
    for(const auto& circ : theCircles){
        ofNoFill();
        ofDrawCircle(circ.center.x,circ.center.y,circ.radius);
       }
    
    ofSetColor( 255 );
    if(isDrawEnabled){
    // センサからの入力を描画
    if(_realsense2.colorEnabled()){
        _realsense2.getColorTex()->draw(1120, 0, 160-20, 90-20);
    }
    if(_realsense2.irEnabled()){
        _realsense2.getIrTex()->draw(1120, 90, 160-20, 90-20);
    }
    if(_realsense2.depthEnabled()){
        _realsense2.getDepthTex()->draw(1120, 180, 160-20, 90-20);
    }
    workTex.draw(1120, 270, 160-20, 90-20);
    
    }
}
//--------------------------------------------------------------
void ofApp::drawSurvey(){
    ofSetColor(255);
    image[1].draw(0,0,width,mainHeight);
    
    float circleSize = 75;
    float spacing = 145;
    
    //各質問に対して選択状態を追跡する配列を初期化
    static std::vector<int>selectedRatings(questionCount,-1);//初期値は未選択(-1)
    
    bool allQuestionsAnswered = true; //全ての質問が答えられたか確認
    
    for(int i = 0; i < questionCount; ++i){
        for(int j = 0; j < ratingCount; ++j){
            float x = 175 + j * (circleSize + spacing);
            float y = 145 + (i * 220);
            ofSetColor(200,200,200,200);
            ofNoFill();
            ofSetLineWidth(5);
            ofDrawCircle(x,y,circleSize); //空の円
            
            //円が選択された処理
            for(const auto& circ : theCircles){
                if(ofDist(circ.center.x,circ.center.y,x,y) < circleSize){
                    
                    //既に選択済みの評価がある場合、その評価を更新
                    if(selectedRatings[i] != j){
                        selectedRatings[i] = j; //新しい評価を記録
                    responses[i] = j; //選択肢を記録
                        allQuestionsAnswered = false;
                        logWritten = false; //新しい選択がされたのでログを書き直す必要がある
                    }
                }
            }
            //選択されている評価の円を緑で塗りつぶし
            if(selectedRatings[i] == j){
                float alpha = (sin(ofGetElapsedTimef() * 5.0f) + 1.0f) * 127.5f;
                ofSetColor(0,255,0,alpha);//選択された円を緑に
                ofNoFill();
                ofSetLineWidth(50);
                ofDrawCircle(x,y,circleSize);
            }
        }
    //評価０を記録しない
    if(selectedRatings[i] == -1){
        allQuestionsAnswered = false;
    }
    }
    
    //ログで記録
    if(allQuestionsAnswered && !logWritten){
        for(int i = 0; i < questionCount; ++i){
            int finalRating = (selectedRatings[i] == -1) ? 0 : (responses[i] + 1);
    
        writeLog2(",Question "+ofToString(i+1)+",Rating, "+ofToString(finalRating));
            
        }
        writeLog2(",__________________________________,");
        logWritten = true;
    }
}
//-------------------------------------------------------------------------------
void ofApp::drawWave(){
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_POINTS);
    
    //基準線の描画(画面中央の基準となるライン)
    
    //波の干渉描画
    for(int y = 0; y < mainHeight; y+=gridSize){
        for(int x = 0;x < width; x+=gridSize){
            float totalIntensity = 0;
            
            //波の計算を共通化
            for(const auto& wave : waves){
                float distance = ofDist(x,y,wave.center.x,wave.center.y);
                
                if(distance < wave.radius){
                float phase = (distance/wave.wavelength)*TWO_PI-(wave.radius/wave.wavelength)*TWO_PI;
                float intensity = wave.amplitude*sin(phase);
                totalIntensity += intensity;
                }
            }
           
            //濃淡の波描画
            int waveCount = static_cast<int>(waves.size());
            int maxWaveCount = 10;
            
            //波の数に基づいてhighColorの上限を制限する係数を計算
            float intensityRange = ofMap(waveCount,1,maxWaveCount,1.0,3.0,true);
            float normalizedIntensity = ofMap(totalIntensity,-intensityRange,intensityRange,0,1,true);
            
            ofColor lowColor(25,25,112);
            ofColor highColor(224,255,255);
            ofColor color = lowColor.lerp(highColor,normalizedIntensity);
            
            mesh.addColor(color);
            mesh.addVertex(ofVec3f(x,y,0));
            
            ofSetColor(color);
            ofDrawRectangle( x - squareSize / 2, y - squareSize / 2,squareSize,squareSize);
            
        }
        }
    glPointSize(gridSize);
    if(scene != 4){
    mesh.draw();
    }
    float alpha = (sin(ofGetElapsedTimef() * 1.0f) + 1.0f) * 127.5f;
    //横線を描画する.
    ofSetColor(0,0,205,alpha);
    ofSetLineWidth(3);
    ofDrawLine(0,mainHeight / 2, width, mainHeight / 2);
   
}
//-------------------------------------------------------------------------------
void ofApp::drawWave2(){
    ofSetColor(137,189,222);//空
    ofDrawRectangle(0, 0, width, subHeight);
    static float smoothedTotalY = 0.0;
    
    //drawSideView
        ofBeginShape();
        for(int x = 0; x < width;x += gridSize){
        float totalY = 0;
            for(const auto& wave : waves){
                float distance = ofDist(x,mainHeight / 2,wave.center.x,wave.center.y);
                if(distance < wave.radius){
                float phase = (distance/wave.wavelength)*TWO_PI-(wave.radius/wave.wavelength)*TWO_PI;
                float y = wave.amplitude*sin(phase) * 25;
                totalY += y;
//                    cout<<"totalY:"<<totalY<<endl;
                    
                    //音
                    if(scene == 2 && selectedContent == 2){
                    float volume = ofMap(abs(totalY),0,5,0.1,2.0);
                    float frequency = ofMap(wavelength,minWavelength,maxWavelength,1.5,0.5);
                        
                    static bool loopSet = false;
                    
                    sound[11].setVolume(volume);
                    sound[11].setSpeed(frequency);
                        
                        if(!loopSet){
                        sound[11].setLoop(true);
                            loopSet = true;
                        }
                        
                        
                    if(!sound[11].isPlaying()){
                    sound[11].play();
                    }
                        cout<<"totalY:"<<totalY<<",volume:"<<volume<<endl;
                    }
                    }
                }
            
            //Y座標を計算して波を描画
           wavePositionY = baseY - totalY;
            ofSetColor(0,0,255);
            ofVertex(x,wavePositionY);
        }
    //波の下を閉じて塗りつぶし
    ofVertex(width,subHeight);
    ofVertex(0,subHeight);
    ofEndShape(true);
    
    //縦波
    float longitudinalBaseY = baseY + 100;//縦並の表示位置
    float spacingBase = 15;//基本間隔
    float x = 0;
    
    while(x < width){
        float totalAmplitude = 0.0;
        for(const auto& wave : waves){
            float distance = ofDist(x,mainHeight / 2,wave.center.x,wave.center.y);
            if(distance < wave.radius){
            float phase = (distance/wave.wavelength)*TWO_PI-(wave.radius/wave.wavelength)*TWO_PI;
                totalAmplitude += wave.amplitude * cos(phase);
            }
    }
        
        float spacing =  spacingBase - ofMap(totalAmplitude,-5,5,-15,15);//密な部分は棒が密集
        spacing = ofClamp(spacing,2,20);
        
        ofSetColor(0,255,0);
        ofSetLineWidth(3);
        ofDrawLine(x,longitudinalBaseY - 30,x,longitudinalBaseY);//縦棒
        x += spacing;
    }
    
    if(selectedContent == 1){
        //横線を描画する.ベースラインより上に描画
        ofSetColor(0,255,0);
        ofSetLineWidth(3);
        ofDrawLine(0,baseY - inner, ofGetWidth(), baseY - inner);
    }

}
//-------------------------------------------------------------------------------
void ofApp::drawScene(){
    ofSetColor(255);
    //時間が経過したか確認
    unsigned long currentTime = ofGetElapsedTimeMillis();
//    if(currentTime - lastSceneChangeTime > 10000){//10秒おき
//        currentScene = (currentScene + 1) % 2;
//        lastSceneChangeTime = currentTime; //時刻を更新
//    }
    //オープニングとエンディング
    if(scene == 0){
        
        if(currentScene == 0){
        if(!sound[4].isPlaying()){
        sound[4].play();
        sound[4].setLoop(3);
        }

        }else if(currentScene == 1){
            image[4].draw(0,0,1280,720);
        }
        
        if(!sound[9].isPlaying() && !hasPlayedSound9){//波をおこそう
        sound[9].play();
        sound[9].setVolume(2);
        hasPlayedSound9 = true;
        }
        
    }else if(scene == 1){
        showAnswer = true;
        if(sound[2].isPlaying()){
            sound[2].stop();
        }
        hasPlayedSound9 = false;

       
    }else if(scene == 2){
//        image[4].draw(0,0,320,180);//256,144
        sound[4].stop();

        if(!sound[8].isPlaying() && !hasPlayedSound8){//波をおこそう
        sound[8].play();
        sound[8].setVolume(2);
        hasPlayedSound8 = true;
        }
        if(!sound[5].isPlaying()){
        sound[5].play();
        sound[5].setVolume(2);
        sound[5].setLoop(2);
        }
        
        
    }else if(scene == 3){
        showAnswer = true;
        hasPlayedSound8 = false;
        if(sound[2].isPlaying()){
            sound[2].stop();
        }

        
        contentPlay = false;
    }else if(scene == 4){
        ofClear( 0,0,139 );//Darkblue
        if(sound[2].isPlaying()){
            sound[2].stop();
        }
        
        if(!sound[4].isPlaying()){
        sound[4].play();
        sound[4].setLoop(3);
        }

        sound[5].stop();
    }
}
//-------------------------------------------------------------------------------
void ofApp::drawMessage(){
    
    if(selectedContent == 1){
    //メッセージの表示
    if(currentStage == 0){
    ofSetColor(waves.size() < 2 ? ofColor(255,255,255) : ofColor(255,0,0)); //条件満たすと赤に変わる
    font.drawString("波を2つ以上起こしてみよう",400,subHeight-50);

    }else if(currentStage == 1){
        ofSetColor(isWaveTouchingInnerLines ? ofColor(255,255,255) : ofColor(255,0,0)); //条件満たすと赤に変わる
        font.drawString("波をたかくしよう",450,subHeight-50);
    
    }else if(currentStage == 2){
            ofSetColor(waves.size() < 3 ? ofColor(255,255,255) : ofColor(255,0,0));
        font.drawString("波を3つ以上起こしてみよう",400,subHeight-70);
            ofSetColor(!isWaveTouchingInnerLines ? ofColor(255,255,255) : ofColor(255,0,0));
        font.drawString("波をひくくしよう",400,subHeight-70 + 60);
        
     }else if(currentStage == 3){
             ofSetColor(waves.size() < 4 ? ofColor(255,255,255) : ofColor(255,0,0));
         font.drawString("波を4つ以上起こしてみよう",400,subHeight-70);
         
             ofSetColor(!isWaveTouchingInnerLines ? ofColor(255,255,255) : ofColor(255,0,0));
         font.drawString("波をひくくしよう",400,subHeight-70 + 60);
     }else if(currentStage == 4){
         ofSetColor(isWaveTouchingInnerLines ? ofColor(255,255,255) : ofColor(255,0,0)); //条件満たすと赤に変わる
         font.drawString("波をたかくしよう",450,subHeight-50);
     
     }
    
    //現在のスコア描画と現在のステージ
    ofSetColor(255);
    font.drawString(ofToString(totalPoints) + "/100Pt",width - 200,subHeight - 50);
    if(currentStage < 5){
    font.drawString(ofToString(currentStage+1) + "/5ステージ",50,50);
    }
    }else if(selectedContent == 2){
        ofSetColor(255);
        font.drawString("音を変えてみよう",width / 2 - 180,50);
    }

}
//-----------------------------------------------------------------------------
void ofApp::drawTimer(){
    
    //タイマーの表示
    if(timerVisible){
        float progress = ofMap(timer, 0, 5.0f, 0, TWO_PI, true);//進行度を角度に変換
        ofSetColor(255,0,0);
        ofSetLineWidth(10);
        ofNoFill();
        ofBeginShape();
        for(float angle = -HALF_PI; angle <= progress - HALF_PI; angle += 0.01){
            float x =   width / 2  + 50 * cos(angle);
            float y =  50 + 45 * sin(angle);
            ofVertex(x,y);
        }
        ofEndShape();
    }
   
       
    
}
//-----------------------------------------------------------------------------
void ofApp::drawComment(){
    
    //0から255の範囲で変化
    //波が描画されないスペース
    if(scene == 2){
        
    if(clear){
        ofSetColor(255,0,0);
    }else if(!clear){
        ofSetColor(0,0,128);
    }
    ofFill();
    }
    
    if(currentScene == 0 && scene == 0){
        ofSetColor(255);
        font.drawString("なみ",ofGetWidth() / 2 - 350,50);
    }
    if(scene == 0){
        float alpha = (sin(ofGetElapsedTimef() * 2.0f) + 1.0f) * 127.5f;
        ofSetColor(255,255,255,alpha);
        font.drawString("QRをかざしてスタート",width / 2 - 210,mainHeight / 2 + 300);
        
    }
    
}
//-----------------------------------------------------------------------------
void ofApp::drawRankings(){
    int x = ofGetWindowWidth() / 2 - 400;
    int y = 180;
    int lineHeight = 50; //各順位の行間

    
    ofSetColor(0);
    ofDrawRectangle(x - 110,y - 70,400,420);//背景
    
    ofSetColor(255);
    font.drawString("ランキング" ,x - 30,y);
    
    //ベクターに保存されているランキングを順に描画
    for(int i = 0;i < 5; i++){
        std::string rankingText;
        int displayedScore = (i < rankings.size()) ? rankings[i] : 0;
        
        if(i < rankings.size()){
            //ランキングにスコアがある場合はそれを表示
            rankingText = ofToString(i + 1) + "位: " + ofToString(rankings[i]) + "Pt";
            if(displayedScore == totalPoints){
                ofSetColor(255,0,0);
            }else{
                ofSetColor(255);
            }
        }else{
            //ランキングにスコアがない場合は0ポイントと表示
            rankingText = ofToString(i + 1) + "位: 0 Pt";
            ofSetColor(255);
        }
        font.drawString(rankingText, x, y + lineHeight * (i + 1));
    }
    //今回のスコア
    ofSetColor(255);
    std::string currentScoreText = "あなたのスコア:" + ofToString(totalPoints) + "Pt";
    font.drawString(currentScoreText,x - 100 ,y + lineHeight * 6);
}
//--------------------------------------------------------------
void ofApp::drawSlider(){
    if(scene == 2){
    //スライダーの描画
//    ofSetColor(180);
//    ofDrawRectangle(sliderX2,velocitySliderY,sliderWidth2,sliderHeight);
//
//    //ハンドルまでの色
//    ofSetColor(100,100,255);
//    float velocityHandleX = ofMap(velocity,2,6,minSliderX2,maxSliderX2);
//        ofSetColor(0,255,0);
//    ofDrawRectangle(sliderX2,velocitySliderY,velocityHandleX - sliderX2,sliderHeight);
    
//    //スライダーのハンドル描画
//    ofSetColor(255,0,0);
//    ofDrawCircle(velocityHandleX,velocitySliderY + sliderHeight / 2,15);
        
//    //画像
//        ofSetColor(255);
//        image[5].draw(sliderX2 - 100,velocitySliderY - 5,80,40);
        
        //波長
        if(selectedContent == 2){
            ofSetColor(180);
            ofDrawRectangle(sliderX,wavelengthSliderY,sliderWidth,sliderHeight);
            ofSetColor(100,100,255);
            float wavelengthHandleX = ofMap(wavelength,minWavelength,maxWavelength,minSliderX,maxSliderX);
            ofDrawRectangle(sliderX,wavelengthSliderY,wavelengthHandleX - sliderX,sliderHeight);
            ofSetColor(255,0,0);
            ofDrawCircle(wavelengthHandleX,wavelengthSliderY + sliderHeight / 2,20);
            ofSetColor(255);
            image[3].draw(10,wavelengthSliderY - 5,80,60);
            
            //波イメージ（波長）
            if(wavelength >= minWavelength && wavelength <= 80){
                selectedImage = image[12];
            }else if(wavelength > 80 && wavelength <= 110){
                selectedImage = image[11];
            }else if(wavelength > 110 && wavelength <= 140){
                selectedImage = image[10];
            }else if(wavelength > 140 && wavelength <= 170){
                selectedImage = image[9];
            }else if(wavelength > 170 && wavelength <= maxWavelength){
                selectedImage = image[8];
            }
            //元も幅と高さ
            float imgW = selectedImage.getWidth();
            float imgH = selectedImage.getHeight();
            
            //アスペクト比を維持して縮小サイズを計算
            float scale = targetHeight / imgH;
            float drawW = imgW * scale;
            float drawH = imgH * scale;
            
            // 描画したい中心のX座標（画面の真ん中）
            float centerX = width / 2;

            // 画像の中心がcenterXに来るように、描画を開始する左上のX座標を計算
            float drawX = centerX - (drawW / 2);

            // 計算した座標を使って描画
            selectedImage.draw(drawX, wavelengthSliderY + 60, drawW, drawH);
        }
        
    }
    
    
}
//--------------------------------------------------------------
void ofApp::drawQuiz(){
    static int selectedAnswer = -1;
    static float startTime = 0.0f;
    static float quizStartTime = -1.0f;
    static int currentQuestion = 0;
    static bool transition = false;
    static bool waitingForNext = false;
    static float waitingStartTime = -1.0f; //待機時間のタイマー
    static bool isCorrect = false;//正解不正解
    
        sleepTime = 4.0f;//クイズ回答後正解発表
   
    if(transition){
        ofBackground(0);
        if(scene == 1){
            scene = 2;
            startPlayTime = ofGetElapsedTimef();
            contentPlay = true;
        }else if(scene == 3){
            scene = 4;
            if(sound[11].isPlaying()){
            sound[11].stop();
            }
        }
        selectedAnswer = -1;
        startTime = 0.0f;
        currentQuestion = 0;
        transition = false;
        
        if(scene == 2){
            currentQuestion = 0;//2
        }else if(scene == 4){
            currentQuestion = 0;
        }
        
        
        return;
    }
    
    //待機中の処理
    if(waitingForNext){
        ofSetColor(100);
        quizImage[currentQuestion].draw(0,0,width,mainHeight);//背景
        ofSetColor(255);
        if(showAnswer){
            kaisetsuImage[currentQuestion].draw(0,0,width,mainHeight);//背景
        if(isCorrect){
            ofSetColor(255);
            image[6].draw( width / 2 - 60, mainHeight / 2 - 200,200,200);
        }else{
            ofSetColor(255);
            image[7].draw( width / 2 - 60, mainHeight / 2 - 200,200,200);
        }
        }
        
        if(ofGetElapsedTimef() - waitingStartTime >= sleepTime){ //x秒間待機
            waitingForNext = false;
            currentQuestion++;
            quizStartTime = -1.0f; //クイズタイムをリセット
//            if((scene == 1 && currentQuestion > 1) || (scene == 3 && currentQuestion > 3)){
                if((scene == 3 && currentQuestion > 1)){
                transition = true; //全問題終了
            }
        }
            return; //次のフレーム
        }
    
    //問題開始時に記録
    if(quizStartTime < 0.0f){
        quizStartTime = ofGetElapsedTimef();
    }
    

    
    quizImage[currentQuestion].draw(0,0,width,mainHeight);
    
    float rectSize = 320;
    float startX = (width - (rectSize * answerCount + (answerCount - 1))) / 2;
    float y = 500;
    
    bool anyCircleInRect = false; //任意の円が現在四角形に重なっているかどうか
    
    for(int i = 0; i < answerCount; ++i){
        float x = startX + i * rectSize;
        ofSetColor(200);
        ofNoFill();
        ofDrawRectangle(x,y,rectSize,rectSize);
    
    //円が四角形に重なっているか
    for(const auto& circ : theCircles){
        if(circ.center.x > x && circ.center.x < x + rectSize &&
           circ.center.y > y && circ.center.y < y + rectSize){
            anyCircleInRect = true; //円が重なっている
            //選択された場合の処理
            if(selectedAnswer != i){
                //新しい選択肢に移動した時，タイマーをリセット
                selectedAnswer = i;
                startTime = ofGetElapsedTimef(); //選択が始まった時間を記録
            }
            if(selectedAnswer == i){
                //塗りつぶし進行度を計算
                float elapsedTime = ofGetElapsedTimef() - startTime;
                float progress = ofClamp(elapsedTime / 1.5f,0.0f,1.0f);
                
                float fillWidth = progress * rectSize;
                ofSetColor(0,255,0,70);
                ofFill();
                ofDrawRectangle(x,y,fillWidth,rectSize);
                
                if(progress >= 1.0f){
                    sound[1].play();
                    //選択確定
                    std::string result;
                    if(currentQuestion == 0 && i == 3){
                        result = "○";
                        isCorrect = true;
                    }else if(currentQuestion == 1 && i == 1){
                        result = "○";
                        isCorrect = true;
                    }else if(currentQuestion == 2 && i == 2){
                        result = "○";
                        isCorrect = true;
                    }else if(currentQuestion == 3 && i == 0){
                        result = "○";
                        isCorrect = true;
                    }else{
                        result = "×";
                        isCorrect = false;
                    }
                    
                    if(showAnswer){
                    if(isCorrect){
                        sound[3].play();
                    }else{
                        sound[12].play();
                    }
                    }
                    
                    float quizElapsedTime = ofGetElapsedTimef() - quizStartTime;
                    
                    
                    writeLog2(",Quiz" + ofToString(currentQuestion + 1)+",Answer," +  ofToString(i + 1) +"," + result+",Time,"+ofToString(quizElapsedTime,2));
                    if(result == "○"){ //正解判定(選択肢１が正解)
                        ofLog() << "○";
                    }else{
                        ofLog() << "×";
                    }
            
                    //問題の切り替え時に初期化
                    selectedAnswer = -1;
                    startTime = 0.0f;
                    waitingForNext = true;
                    waitingStartTime = ofGetElapsedTimef(); //待機タイマーを開始
                    return;//次のフレームへ
                }
            }
        }
    }
  }
    //円がどの四角形にも重なっていない場合，選択状態をリセット
    if(!anyCircleInRect){
        selectedAnswer = -1;
        startTime = 0.0f;
    }
    
}
//--------------------------------------------------------------
void ofApp::drawMain(){
    drawWave();
    drawScene();
    if(scene == 1 || scene == 3){
        drawQuiz();
    }
    if(scene == 4){
    drawSurvey();
    }
    drawSelect();
    drawComment();
    drawSlider();
    if(scene == 2 && selectedContent == 1 && currentStage == 5){
        drawRankings();
    }
    
    if(isDrawEnabled){
        ofSetColor(255);
    ofDrawBitmapString(ofGetFrameRate(), 10, 10);
    }
    
    //いつでもクイズの解説を見せる(key == x,y,z)
    if(showKaisetsu1){
        kaisetsuImage[0].draw(0,0,width,mainHeight);
    }else if(showKaisetsu2){
        kaisetsuImage[1].draw(0,0,width,mainHeight);
    }else if(showKaisetsu3){
        kaisetsuImage[2].draw(0,0,width,mainHeight);
    }else if(showHint){ //'h'key
        image[4].draw(0,0,width,mainHeight);
    }
    
}
//--------------------------------------------------------------
void ofApp::drawSub(){
    drawWave2();
    if(scene == 2){
    drawMessage();
        if(selectedContent == 1){
        drawTimer();
        }else if(selectedContent == 2){
            
        }
    }
    
}
//--------------------------------------------------------------
void ofApp::draw(){
    //ウィンドウサイズと現在のサイズを取得
float windowWidth = ofGetWidth();
float windowHeight = ofGetHeight();
   
//元のサイズを取得
float originalWidth = width;
float originalHeight = height;
   
//ウィンドウのアスペクト比とアスペクト比を計算
float windowAspect = windowWidth / windowHeight;
float imageAspect = originalWidth / originalHeight;
   
//スケーリング因子を計算
float scale;
if(windowAspect > imageAspect){
//ウィンドウが横長の場合
scale = windowHeight / originalHeight; //高さに基づく
}else{
//ウィンドウが縦長または同じアスペクト比の場合
scale = windowWidth / originalWidth; //幅に基づくスケール
}
   
//描画位置を計算して中央に配置
float scaleWidth = originalWidth * scale;
float scaleHeight = originalHeight * scale;
float offsetX1 = (windowWidth - scaleWidth) / 2;
float offsetY1 = (windowHeight - scaleHeight) / 2;
   
//スケールを適用
ofPushMatrix();
ofTranslate(offsetX1,offsetY1); //中央に配置
ofScale(scale,scale);
    
    ofBackground(25,25,112);
    
    //上さぶ
       mainFbo.draw(0,subHeight,width,mainHeight);
       subFbo.draw(0,0,width,subHeight);
   
        
    ofPopMatrix();
    //GUI描画
    if(isDrawEnabled){
       gui.draw();
    }
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if(key == 'a'){
        scene = 2;
    }else if(key == 'c'){
        screenshotCount++;
        
        ScreenShot.grabScreen(0 ,0, 1280 * 1.2, 720 * 1.2);
        std::string fileName = "screenshots/screenshot_" + std::to_string(screenshotCount) + ".png";
        ScreenShot.save(fileName);
    }else if(key == 'u'){
        if(interval < maxInterval){
            interval++;
        }
     }else if(key == 'd'){
         if(interval > minInterval){
             interval--;
         }
     }else if(key == 'e'){
         if(scene == 2){
             currentStage = 5;
         }
         
     }else if(key == 'g'){
         timer = 0.0f; //タイマーのリセット
         conditionMetDuration = 0.0f; //条件が満たされた時間をリセット
     }else if(key == 'h'){
         showHint = !showHint;
     }else if(key == 'm'){
        if(scene == 2){
//             scene = 3;
            scene = 4;
            if(sound[11].isPlaying()){
            sound[11].stop();
            }
         }
     }else if(key == 'o'){
         scene = 0;
         currentStage = 0;
         selectedContent = 0;
         waves.clear();
         contentPlay = false;
         if(sound[11].isPlaying()){
         sound[11].stop();
         }
         writeLog("__________________________________");
         writeLog2(",__________________________________,");
     }else if(key == 'p'){
         isPaused = !isPaused;
     }else if(key == 'q'){
         scene = 4;
         if(sound[11].isPlaying()){
         sound[11].stop();
         }
     }else if(key == 'r'){
       waves.clear();
   }else if(key == 't'){
        if(scene == 1){
            scene = 2;
        }else if(scene == 3){
            scene = 0;
        }
   }else if(key == 'x'){
       showKaisetsu1 = !showKaisetsu1;
      
   }else if(key == 'y'){
       showKaisetsu2 = !showKaisetsu2;
       
   }else if(key == 'z'){
       showKaisetsu3 = !showKaisetsu3;
       
   }else if(key == ' '){
        isDrawEnabled = !isDrawEnabled;
   }else if(key == 'd'){
       volumeDown = !volumeDown;
   }
    
    
    if(key == OF_KEY_UP){
        offsetY -= 20;//元20
    }
    if(key == OF_KEY_DOWN){
        offsetY += 20;
    }
    if(key == OF_KEY_LEFT){
        offsetX -= 20;
    }
    if(key == OF_KEY_RIGHT){
        offsetX += 20;
    }
    
        
        //キオスク
        if(scene==0){
            
            if (key == 'S'||key == 's'||key==' ') {
                    QR_1 = 5;
                    QR_2 = 1;
                    UserNum = "";
                }
                if (QR_2 == 1) {
                    if ((key == 'C' && QR_1 == 4) || (key == 'c' && QR_1 == 5)) {
                        QR_1--;
                    } else if ('0' <= key && key <= '9' && QR_1 != 5) {
                        UserNum += key;
                        QR_1--;
                    } else if ('A' <= key && key <= 'F' && QR_1 != 5) {
                        UserNum += key;
                        QR_1--;
                    }
                }
//            cout<<"User:"<<UserNum<<endl;
        }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}
//--------------------------------------------------------------
//条件をチェックして次のステージへ進むかどうかを判断する
void ofApp::checkConditions(){
    if(scene == 2){
    bool conditionMet = false;

    if(currentStage == 0 ){
        bool waveCountCondition = (waves.size() >= 2);
        if(waveCountCondition){
            conditionMet = true;
        }
            
    }else if(currentStage == 1 ){
        bool waveTouchCondition = !checkWaveTouchingInnerLines();//波が内側の線に達する場合(たかくしよう)
        isWaveTouchingInnerLines = !waveTouchCondition; //フラグの更新
        if(waveTouchCondition){
            conditionMet = true;
            
        }
    }else if(currentStage == 2){
        bool waveCountCondition = (waves.size() >= 3);
        bool waveTouchCondition = checkWaveTouchingInnerLines();//波が内側の線に達さない場合（低く）
        isWaveTouchingInnerLines = waveTouchCondition; //フラグの更新
        
        if(waveCountCondition && waveTouchCondition){
            conditionMet = true;
           
        }
        }else if(currentStage == 3){
            bool waveCountCondition = (waves.size() >= 4);
            bool waveTouchCondition = checkWaveTouchingInnerLines();//波が内側の線に達さない場合
            isWaveTouchingInnerLines = waveTouchCondition; //フラグの更新
            
            if(waveCountCondition && waveTouchCondition){
                conditionMet = true;
                
            }
        }else if(currentStage == 4){
            bool waveTouchCondition = !checkWaveTouchingInnerLines();//波が内側の線に達する場合
            isWaveTouchingInnerLines = !waveTouchCondition; //フラグの更新
            if(waveTouchCondition){
                conditionMet = true;
                
            }
        }else if(currentStage == 5){
            
        }
        
    if(conditionMet){
            if(!timerVisible){
                timer = 0.0f;
                timerVisible = true; //タイマーを表示する
                sound[0].play();
            }
                
            conditionMetDuration += ofGetLastFrameTime(); //条件が満たされている時間をカウント
            if(conditionMetDuration >= maxConditionMetTime){
                clear = true;
                advanceToNextStage(); //条件が一定時間満たされたら次のステージへ進む
            }
            timer += ofGetLastFrameTime(); //タイマーを更新

            timerVisible = true; //タイマーを非表示にする
        }else{
            conditionMetDuration = 0; //条件が満たされていない場合はタイマーをリセット
            timer = 0; //タイマーのカウントもリセット
            clear = false;
            timerVisible = false; //タイマーを非表示にする
        }
    }
    }
bool ofApp::checkWaveTouchingInnerLines(){
    
float innerLineUpper = baseY - inner;

    for(int x = 0; x < width;x++){
    float totalY = 0;
        for(const auto& wave : waves){
            float distance = ofDist(x,baseY,wave.center.x,wave.center.y);
            if(distance < wave.radius){
            float phase = (distance/wave.wavelength)*TWO_PI-(wave.radius/wave.wavelength)*TWO_PI;
            float y = wave.amplitude*sin(phase) * 27;//27が一番いいかも
            totalY += y;
            }
            }
    wavePositionY = baseY - totalY;
    
    //波が内側の下線に触れたかどうかを確認
//    if(wavePositionY < innerLineUpper){
//        return false; //どこかで触れていない場合はfalse(元)
//    }
        if(wavePositionY > innerLineUpper){
            return true; //どこかで触れている場合はtrue
        }
}
//return true;(元)
    return false;
}
//---------------------------------------------------------
int ofApp::calculatePoints(int stage, int interval){
    int points = 0;
    switch(stage){
        case 2:
            if(interval <= 5){
            points = ofMap(interval,1,5,5,30,true); //元0~25pt
            }
            break;
        case 3:
            if(interval <= 20){
            points = ofMap(interval,5,20,30,5,true);
            }
            break;
        case 4:
            if(interval <= 20){
            points = ofMap(interval,5,20,30,5,true);
            }
            break;
        case 5:
            if(interval <= 5){
            points = ofMap(interval,1,5,5,30,true);
            }
            break;
    }
    return points;
}
//---------------------------------------------------------
//次のステージに進む
void ofApp::advanceToNextStage(){
    sound[3].play();
    ofSetColor(255);
    
        currentStage++;
        conditionMetDuration = 0;
        timer = 0.0f;
    
    int points = calculatePoints(currentStage,interval);
    totalPoints += points;
    
    writeLog2(",Stage," + ofToString(currentStage) + " ,Points," + ofToString(points));
    
    if(currentStage == 5){
    writeLog2(",Total Points," + ofToString(totalPoints) +"Pt,");
        updateRankings(totalPoints); //更新
    }
        //新しいステージの条件に応じてメッセージを更新する
        if(currentStage == 0){
        }else if(currentStage == 1){
            totalPoints = 0;//ランキング用，
            writeLog2( ",Interval.1," + ofToString(interval) );
            interval = 5;
            waves.clear();
        }else if(currentStage == 2){
            writeLog2( ",Interval.2," + ofToString(interval) );
            interval = 5;
        }else if(currentStage == 3){
            writeLog2( ",Interval.3," + ofToString(interval) );
            interval = 5;
        }else if(currentStage == 4){
            writeLog2( ",Interval.4," + ofToString(interval) );
            interval = 5;
            waves.clear();
        }else if(currentStage == 5){
            writeLog2( ",Interval.5," + ofToString(interval) );
        }
    
    }

//--------------------------------------------------------------
void ofApp::exit(){
   _realsense2.exit();
}
//--------------------------------------------------------------
void ofApp::mesSend(string QRNumber, string mes){
    if (!tcpClient.isConnected()) {
        tcpClient.setup(IP, PORT);
        tcpClient.setMessageDelimiter("\n");
    }
 
    if (tcpClient.isConnected()) {
        tcpClient.send("QRtouch "+ofGetTimestampString("%F %T")+" "+ofToString(MacNum, 3, '0')+" "+QRNumber+" "+mes);
        tcpClient.close();
 
//        送信成功時のログ
        cout<<"QRtouch "+ofGetTimestampString("%F %T")+" "+ofToString(MacNum, 3, '0')+" "+QRNumber+" "+mes <<endl;
    } else {
 
//        送信失敗時のログ
 
    }
}
//--------------------------------------------------------------
// 研究室wikiサーバに文字列txtを送る、ネット接続必要
 
void ofApp::sendText2server(string txt){
 
    // 記録サーバとの通信用（いじらない）
    string com = "curl -s -m 3";
    string server = "'http://colors.ise.ibaraki.ac.jp/IoTproj/log.php?";
    string auth = "-u 'iotproj:BrVQxevZ'";
    string who = "who=";
    string ID = "ID=";
 
    // 各自でプログラムごとに設定
    // 使えない文字 &, %, =
    who += "endo"; // 自分の名前にする、空白OK
    ID += "tangible"; // 自分のマシン/プロジェクト名にする、空白OK
 
    ofStringReplace( who, " ", "%20" ); // 空白を変換 for cURL
    ofStringReplace( ID, " ", "%20" );
    ofStringReplace( txt, " ", "%20" );
        
    string fullURL = com + " " + auth + " " + server + who +"&"+ ID +"&line="+txt+"'&";
 
    cout << fullURL << endl;
    ofSystem( fullURL );
 
}

