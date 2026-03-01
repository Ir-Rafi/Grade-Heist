// CombinationLockOverlay.cpp
#include "CombinationLockOverlay.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

CombinationLockOverlay::CombinationLockOverlay(unsigned w, unsigned h)
: currentState(State::Inactive)
, taskPassed(false)
, autoClose(false)
, rng(std::random_device{}())
{
    if (!m_rt.create(w, h))
        std::cerr << "Error: cannot create CombinationLockOverlay\n";
    m_overlay.setTexture(m_rt.getTexture());
    setupUI();
}

void CombinationLockOverlay::setupUI() {
    if (!font.loadFromFile("arial.ttf"))
        std::cerr << "Warning: could not load arial.ttf\n";

    correctCode       = "745268";
    hintText          = "Put all the numbers u got throughout the whole journey";
    maxDigits         = 6;
    maxAttempts       = 2;
    attemptsRemaining = maxAttempts;

    backgroundColor   = sf::Color(15,15,25);
    successColor      = sf::Color(0,255,100);
    failColor         = sf::Color(255,50,50);

    setupBackground();
    setupVisualElements();
    setupTexts();
}

void CombinationLockOverlay::setupBackground() {
    backgroundParticles.clear();
    std::uniform_int_distribution<int> distSize(2,5), distX(0,m_rt.getSize().x), distY(0,m_rt.getSize().y), distCol(100,200);
    for (int i = 0; i < 20; ++i) {
        sf::CircleShape p(distSize(rng));
        p.setPosition(distX(rng), distY(rng));
        p.setFillColor({
            (sf::Uint8)distCol(rng),
            (sf::Uint8)distCol(rng),
            (sf::Uint8)std::min(255,150 + distCol(rng)/2),
            (sf::Uint8)(50 + distCol(rng)%100)
        });
        backgroundParticles.push_back(p);
    }
}

void CombinationLockOverlay::setupVisualElements() {
    lockFrame.setSize({450.f,400.f});
    lockFrame.setPosition(175.f,100.f);
    lockFrame.setFillColor({40,40,60,200});
    lockFrame.setOutlineThickness(6.f);
    lockFrame.setOutlineColor({120,120,140});

    lockDial.setRadius(60.f);
    lockDial.setPosition(340.f,130.f);
    lockDial.setFillColor({50,50,70});
    lockDial.setOutlineThickness(4.f);
    lockDial.setOutlineColor(sf::Color::White);

    codeDisplay.setSize({350.f,60.f});
    codeDisplay.setPosition(225.f,220.f);
    codeDisplay.setFillColor({20,20,20});
    codeDisplay.setOutlineThickness(3.f);
    codeDisplay.setOutlineColor({0,255,0});

    digitBoxes.clear();
    digitTexts.clear();
    for (int i=0;i<maxDigits;++i) {
        sf::RectangleShape box({45.f,40.f});
        box.setPosition(245.f + i*50.f, 230.f);
        box.setFillColor({30,30,30});
        box.setOutlineThickness(2.f);
        box.setOutlineColor({0,200,0});
        digitBoxes.push_back(box);

        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(24);
        t.setFillColor({0,255,0});
        t.setPosition(255.f + i*50.f, 235.f);
        digitTexts.push_back(t);
    }

    numberButtons.clear();
    buttonTexts.clear();
    // 1–9
    for (int i=1;i<=9;++i) {
        int idx=i-1, col=idx%3, row=idx/3;
        sf::RectangleShape btn({50.f,50.f});
        btn.setPosition(100.f + col*60.f, 350.f + row*60.f);
        btn.setFillColor({60,80,100});
        btn.setOutlineThickness(2.f);
        btn.setOutlineColor(sf::Color::White);
        numberButtons.push_back(btn);

        sf::Text txt;
        txt.setFont(font);
        txt.setString(std::to_string(i));
        txt.setCharacterSize(24);
        txt.setFillColor(sf::Color::White);
        auto b=btn.getGlobalBounds(), tb=txt.getLocalBounds();
        txt.setPosition(
            b.left + (b.width - tb.width)/2.f - tb.left,
            b.top  + (b.height - tb.height)/2.f - tb.top
        );
        buttonTexts.push_back(txt);
    }
    // 0
    {
        sf::RectangleShape btn({50.f,50.f});
        btn.setPosition(100.f + 60.f, 350.f + 3*60.f);
        btn.setFillColor({60,80,100});
        btn.setOutlineThickness(2.f);
        btn.setOutlineColor(sf::Color::White);
        numberButtons.push_back(btn);

        sf::Text txt;
        txt.setFont(font);
        txt.setString("0");
        txt.setCharacterSize(24);
        txt.setFillColor(sf::Color::White);
        auto b=btn.getGlobalBounds(), tb=txt.getLocalBounds();
        txt.setPosition(
            b.left + (b.width - tb.width)/2.f - tb.left,
            b.top  + (b.height - tb.height)/2.f - tb.top
        );
        buttonTexts.push_back(txt);
    }

    enterButton.setSize({120.f,50.f});
    enterButton.setPosition(250.f,320.f);
    enterButton.setFillColor({0,150,0});
    enterButton.setOutlineThickness(3.f);
    enterButton.setOutlineColor(sf::Color::White);

    clearButton.setSize({120.f,50.f});
    clearButton.setPosition(430.f,320.f);
    clearButton.setFillColor({150,0,0});
    clearButton.setOutlineThickness(3.f);
    clearButton.setOutlineColor(sf::Color::White);
}

void CombinationLockOverlay::setupTexts() {
    titleText.setFont(font);
    titleText.setString("Welcome to the Last Stage");
    titleText.setCharacterSize(28);
    titleText.setFillColor({255,100,100});
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);
    titleText.setPosition(200.f,20.f);

    hintDisplayText.setFont(font);
    hintDisplayText.setString("HINT: " + hintText);
    hintDisplayText.setCharacterSize(16);
    hintDisplayText.setFillColor({255,255,100});
    hintDisplayText.setOutlineColor(sf::Color::Black);
    hintDisplayText.setOutlineThickness(1.f);
    hintDisplayText.setPosition(50.f,120.f);

    codeDisplayText.setFont(font);
    codeDisplayText.setString("Enter 6-digit code:");
    codeDisplayText.setCharacterSize(18);
    codeDisplayText.setFillColor(sf::Color::White);
    codeDisplayText.setOutlineColor(sf::Color::Black);
    codeDisplayText.setOutlineThickness(1.f);
    codeDisplayText.setPosition(280.f,200.f);

    instructionText.setFont(font);
    instructionText.setString("Use keyboard or click digits - order doesn't matter");
    instructionText.setCharacterSize(14);
    instructionText.setFillColor({100,255,255});
    instructionText.setOutlineColor(sf::Color::Black);
    instructionText.setOutlineThickness(1.f);
    instructionText.setPosition(180.f,290.f);

    enterButtonText.setFont(font);
    enterButtonText.setString("ENTER");
    enterButtonText.setCharacterSize(16);
    enterButtonText.setFillColor(sf::Color::White);
    enterButtonText.setOutlineColor(sf::Color::Black);
    enterButtonText.setOutlineThickness(1.f);
    enterButtonText.setPosition(285.f,335.f);

    clearButtonText.setFont(font);
    clearButtonText.setString("CLEAR");
    clearButtonText.setCharacterSize(16);
    clearButtonText.setFillColor(sf::Color::White);
    clearButtonText.setOutlineColor(sf::Color::Black);
    clearButtonText.setOutlineThickness(1.f);
    clearButtonText.setPosition(465.f,335.f);

    attemptText.setFont(font);
    attemptText.setCharacterSize(18);
    attemptText.setFillColor(sf::Color::Yellow);
    attemptText.setOutlineColor(sf::Color::Black);
    attemptText.setOutlineThickness(1.f);
    attemptText.setPosition(500.f,280.f);

    resultText.setFont(font);
    resultText.setCharacterSize(22);
    resultText.setOutlineColor(sf::Color::Black);
    resultText.setOutlineThickness(2.f);
    resultText.setPosition(150.f,420.f);

    exitText.setFont(font);
    exitText.setString("Press ESC or ENTER to close");
    exitText.setCharacterSize(16);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(1.f);
    exitText.setPosition(320.f,500.f);
}

void CombinationLockOverlay::start() {
    enteredCode.clear();
    attemptsRemaining = maxAttempts;
    taskPassed       = false;
    autoClose        = false;
    currentState     = State::HintDisplay;
    gameClock.restart();
    particleClock.restart();
    updateCodeDisplay();
}

void CombinationLockOverlay::handleEvent(const sf::Event& e) {
    if (currentState == State::Inactive) return;

    // ESC closes
    if (e.type==sf::Event::KeyPressed && e.key.code==sf::Keyboard::Escape) {
        currentState = State::Inactive;
        return;
    }
    // ENTER after done closes
    if (currentState==State::Completed &&
        e.type==sf::Event::KeyPressed &&
        e.key.code==sf::Keyboard::Enter)
    {
        currentState = State::Inactive;
        return;
    }

    // HintDisplay → start on any click/key
    if (currentState==State::HintDisplay) {
        if (e.type==sf::Event::MouseButtonPressed ||
            e.type==sf::Event::KeyPressed)
        {
            currentState = State::Playing;
            gameClock.restart();
        }
        return;
    }

    // PLAYING: handle mouse & keyboard
    if (currentState==State::Playing) {
        if (e.type==sf::Event::MouseButtonPressed &&
            e.mouseButton.button==sf::Mouse::Left)
        {
            sf::Vector2f global(
              float(e.mouseButton.x),
              float(e.mouseButton.y)
            );
            sf::Vector2f overlayPos = m_overlay.getPosition();
            sf::Vector2f local     = global - overlayPos;
            handleMouseClick(int(local.x), int(local.y));
        }
        else if (e.type==sf::Event::KeyPressed) {
            handleKeyPress(e.key.code);
        }
    }
}

void CombinationLockOverlay::handleMouseClick(int x,int y) {
    if (currentState!=State::Playing) return;

    // digits
    for(int i=0; i<10; ++i){
        if(numberButtons[i].getGlobalBounds().contains(x,y)){
            if(int(enteredCode.size())<maxDigits){
                char d = (i<9? char('1'+i) : '0');
                enteredCode.push_back(d);
                updateCodeDisplay();
            }
            return;
        }
    }
    // enter
    if(enterButton.getGlobalBounds().contains(x,y)){
        if(int(enteredCode.size())==maxDigits)
            checkCombination();
        return;
    }
    // clear
    if(clearButton.getGlobalBounds().contains(x,y)){
        enteredCode.clear();
        updateCodeDisplay();
    }
}

void CombinationLockOverlay::handleKeyPress(sf::Keyboard::Key k){
    if(currentState!=State::Playing) return;

    if(k>=sf::Keyboard::Num0 && k<=sf::Keyboard::Num9){
        if(int(enteredCode.size())<maxDigits){
            enteredCode.push_back(char('0'+(k-sf::Keyboard::Num0)));
            updateCodeDisplay();
        }
    }
    else if(k>=sf::Keyboard::Numpad0 && k<=sf::Keyboard::Numpad9){
        if(int(enteredCode.size())<maxDigits){
            enteredCode.push_back(char('0'+(k-sf::Keyboard::Numpad0)));
            updateCodeDisplay();
        }
    }
    else if(k==sf::Keyboard::BackSpace||k==sf::Keyboard::Delete){
        if(!enteredCode.empty()){
            enteredCode.pop_back();
            updateCodeDisplay();
        }
    }
    else if(k==sf::Keyboard::Enter){
        if(int(enteredCode.size())==maxDigits)
            checkCombination();
    }
}

void CombinationLockOverlay::updateCodeDisplay(){
    for(int i=0;i<maxDigits;++i){
        digitTexts[i].setString(
          i < int(enteredCode.size())
            ? std::string(1, enteredCode[i])
            : "_"
        );
    }
}

bool CombinationLockOverlay::checkDigitsMatch(
    const std::string& e, const std::string& c)
{
    auto ee=e, cc=c;
    std::sort(ee.begin(),ee.end());
    std::sort(cc.begin(),cc.end());
    return ee==cc;
}

void CombinationLockOverlay::checkCombination(){
    // success?
    if(checkDigitsMatch(enteredCode, correctCode)){
        taskPassed   = true;
        currentState = State::Completed;
        resultText.setString("✅ PASS! (8)\nDoor unlocked!");
        resultText.setFillColor(successColor);
        lockFrame.setOutlineColor(successColor);
        lockDial.setOutlineColor(successColor);
        codeDisplay.setOutlineColor(successColor);
        for(auto& b:digitBoxes) b.setOutlineColor(successColor);
    }
    else {
        // decrement
        --attemptsRemaining;
        if(attemptsRemaining<=0){
            taskPassed   = false;
            currentState = State::Completed;
            resultText.setString("❌ FAIL!\nNo attempts left");
            resultText.setFillColor(failColor);
            lockFrame.setOutlineColor(failColor);
            lockDial.setOutlineColor(failColor);
            codeDisplay.setOutlineColor(failColor);
            for(auto& b:digitBoxes) b.setOutlineColor(failColor);
        }
        else {
            // still playing
            resultText.setString(
              "Wrong! " + std::to_string(attemptsRemaining) + " left"
            );
            resultText.setFillColor(sf::Color::Yellow);
            lockFrame.setOutlineColor(failColor);
            lockDial.setOutlineColor(failColor);
            codeDisplay.setOutlineColor(failColor);
            for(auto& b:digitBoxes) b.setOutlineColor(failColor);
            enteredCode.clear();
            updateCodeDisplay();
            gameClock.restart();
            return; // <— back to playing
        }
    }

    // if we reach here, it’s completed (success or final fail)
    finishClock.restart();
    autoClose = true;
}

void CombinationLockOverlay::update(float /*dt*/){
    updateBackground();
    if(currentState==State::Playing){
        attemptText.setString(
          "Attempts: "+std::to_string(attemptsRemaining)
          +"/"+std::to_string(maxAttempts)
        );
        if(gameClock.getElapsedTime().asSeconds()>1.f
           && attemptsRemaining>0)
        {
            lockFrame.setOutlineColor({120,120,140});
            lockDial.setOutlineColor(sf::Color::White);
            codeDisplay.setOutlineColor({0,255,0});
            for(auto& b:digitBoxes) 
                b.setOutlineColor({0,200,0});
            resultText.setString("");
        }
    }
    if(currentState==State::Completed && autoClose){
        if(finishClock.getElapsedTime().asSeconds()>=6.f)
            currentState = State::Inactive;
    }
}

void CombinationLockOverlay::updateBackground(){
    float dt = particleClock.restart().asSeconds();
    std::uniform_int_distribution<int> dx(20,50);
    for(auto& p:backgroundParticles){
        auto pos=p.getPosition();
        pos.x += dx(rng)*dt;
        pos.y += std::sin(pos.x*0.01f)*20.f*dt;
        if(pos.x>m_rt.getSize().x+20){
            pos.x=-20;
            pos.y=rng()%m_rt.getSize().y;
        }
        p.setPosition(pos);
    }
}

void CombinationLockOverlay::draw(sf::RenderWindow& window){
    if(currentState==State::Inactive) return;

    m_rt.clear({0,0,0,180});
    for(auto& p:backgroundParticles) m_rt.draw(p);
    m_rt.draw(titleText);
    if(currentState==State::HintDisplay)
        m_rt.draw(hintDisplayText);

    // UI
    m_rt.draw(lockFrame);
    m_rt.draw(lockDial);
    m_rt.draw(codeDisplay);
    m_rt.draw(codeDisplayText);
    for(auto& box:digitBoxes) m_rt.draw(box);
    for(auto& txt:digitTexts) m_rt.draw(txt);
    for(auto& btn:numberButtons) m_rt.draw(btn);
    for(auto& txt:buttonTexts) m_rt.draw(txt);
    m_rt.draw(enterButton);
    m_rt.draw(clearButton);
    m_rt.draw(enterButtonText);
    m_rt.draw(clearButtonText);
    m_rt.draw(instructionText);
    m_rt.draw(attemptText);
    if(currentState==State::Completed){
        m_rt.draw(resultText);
        m_rt.draw(exitText);
    }

    m_rt.display();
    auto ws=window.getSize(), rs=m_rt.getSize();
    m_overlay.setPosition(
      (ws.x-rs.x)/2.f,
      (ws.y-rs.y)/2.f
    );
    window.draw(m_overlay);
}

bool CombinationLockOverlay::isActive()   const {
    return currentState==State::HintDisplay
        || currentState==State::Playing;
}
bool CombinationLockOverlay::isFinished() const {
    return currentState==State::Completed;
}
bool CombinationLockOverlay::passed()     const {
    return taskPassed;
}
