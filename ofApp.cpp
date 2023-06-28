#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);

	this->font.loadFont("fonts/Kazesawa-Bold.ttf", 150, true, true, true);
	
	ofFbo fbo;
	fbo.allocate(ofGetWidth(), ofGetHeight());
	fbo.begin();
	ofTranslate(ofGetWidth() * 0.5, ofGetHeight() * 0.5);
	ofClear(0);
	ofSetColor(0);

	string word = std::to_string((ofGetFrameNum() / 30) % 10);
	word = "Arrow";
	font.drawString(word, font.stringWidth(word) * -0.5, font.stringHeight(word) - 300);

	fbo.end();
	auto span = 10;
	ofPixels pixels;
	fbo.readToPixels(pixels);
	for (int x = 60; x < 700; x += span) {

		for (int y = 0; y < 720; y += span) {

			ofColor color = pixels.getColor(x, y);
			if (color != ofColor(0, 0)) {

				this->location_list.push_back(glm::vec3(x - ofGetWidth() * 0.5, y - ofGetHeight() * 0.25, 0));
			}
		}
	}

}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	int span = 10;
	for (auto& location : this->location_list) {

		auto x = location.x;
		auto y = location.y;

		auto p = ofGetFrameNum() % 120;
		auto next_p = (ofGetFrameNum() + 1) % 120;

		auto param = p < 20 ? 0 : p < 60 ? ofMap(p, 20, 60, 0, 1) : p < 100 ? 1 : ofMap(p, 100, 120, 1, 0);
		auto next_param = next_p < 20 ? 0 : next_p < 60 ? ofMap(next_p, 20, 60, 0, 1) : next_p < 100 ? 1 : ofMap(next_p, 100, 120, 1, 0);

		auto noise_seed = glm::vec2(ofRandom(1000), ofRandom(1000));

		auto param_x = ofMap(ofNoise(noise_seed.x, ofGetFrameNum() * 0.015), 0, 1, span * -15, span * 15) * param;
		auto param_y = ofMap(ofNoise(noise_seed.y, ofGetFrameNum() * 0.015), 0, 1, span * -15, span * 15) * param;

		auto next_param_x = ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + 1) * 0.015), 0, 1, span * -15, span * 15) * next_param;
		auto next_param_y = ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + 1) * 0.015), 0, 1, span * -15, span * 15) * next_param;

		if (param > 0) {

			this->draw_arrow(glm::vec2(x + param_x, y + param_y), glm::vec2(x + next_param_x, y + next_param_y), span * 1.2, ofColor(0), ofColor(255));
		}
		else {

			ofSetColor(0);
			ofFill();
			ofDrawCircle(x + param_x, y + param_y, span * 0.35);

			ofSetColor(255);
			ofNoFill();
			ofDrawCircle(x + param_x, y + param_y, span * 0.35);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw_arrow(glm::vec2 location, glm::vec2 next_location, float size, ofColor fill_color, ofColor no_fill_color) {

	auto angle = std::atan2(next_location.y - location.y, next_location.x - location.x);

	ofSetColor(fill_color);
	ofFill();
	ofBeginShape();
	ofVertex(glm::vec2(size * 0.8 * cos(angle), size * 0.8 * sin(angle)) + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) + location);
	ofEndShape(true);

	ofBeginShape();
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 + location);
	ofEndShape(true);

	ofSetColor(no_fill_color);
	ofNoFill();
	ofBeginShape();
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) + location);
	ofVertex(glm::vec2(size * 0.8 * cos(angle), size * 0.8 * sin(angle)) + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 + location);
	ofEndShape();

	ofBeginShape();
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5 + location);
	ofVertex(glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 + location);
	ofEndShape();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}