#pragma once

struct STATE
{
	bool can;	//～が可能か
	bool is;	//～中か
	int interval;	//～までの必要フレーム
	int timer;	//～からの経過フレーム

	bool Start() {
		if(can){
			can = false;
			is = true;
			timer = interval;
			return true;
		}
		return false;
	}

	void Intervel(bool isReset = false) {
		if (!can) {
			if (timer <= 0) {
				can = true;
				is = isReset;
			}
			else {
				timer--;
			}
		}
	}
};