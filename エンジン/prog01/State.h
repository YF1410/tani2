#pragma once

struct STATE
{
	bool can;	//�`���\��
	bool is;	//�`����
	int interval;	//�`�܂ł̕K�v�t���[��
	int timer;	//�`����̌o�߃t���[��

	bool Start() {
		if(can){
			can = false;
			is = true;
			timer = interval;
			return true;
		}
		return false;
	}

	void Intervel() {
		if (!can) {
			if (timer <= 0) {
				can = true;
			}
			else {
				timer--;
			}
		}
	}
};