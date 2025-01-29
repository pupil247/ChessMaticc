#include "Leds.h"
#include "esp_log.h"
Leds::Leds(uint8_t gpio,uint8_t ledCount) : gpio_num(gpio),ledCount(ledCount){
	frameDelay = 1;
    pixels = new Pixels((gpio_num_t)gpio_num, ledCount, Pixels::StripType::ws6812, RMT_CHANNEL_1, 2.8);
	currentEffect = nullptr;
}

void Leds::setBoard(){
	Effect *newEffect = EffectFactory::CreateEffect("static", ledCount, frameDelay);
	pixels->Clear();
	delete currentEffect;
	currentEffect = newEffect;
}

void Leds::setEffect(std::string effect){
	printf("setting effect: \"%s\"\n", effect.c_str());
	std::string effectName = effect.substr(0, effect.find(':'));
	if(effectName == "snake" || effectName == "police" || effectName == "static" || effectName == "solid" || effectName =="stars" || effectName == "rainbow"){
		Effect *newEffect = EffectFactory::CreateEffect(effect, ledCount, frameDelay);
		pixels->Clear();
		//newEffect->Run(this->pixels);
		if(currentEffect != nullptr)
			delete (currentEffect);
		currentEffect = newEffect;
	}
	
	//currentEffect.reset(newEffect);
	
}

void Leds::setFrameDelay(uint8_t delayMS){
	this->frameDelay = delayMS;	
}

void Leds::run(){
	this->currentEffect->Run(this->pixels);
}

