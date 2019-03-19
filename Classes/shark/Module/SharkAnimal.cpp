#include "SharkAnimal.h"




SharkAnimal::SharkAnimal() : m_act(nullptr)
{
}


SharkAnimal::~SharkAnimal()
{
}


void SharkAnimal::PlayAct(int posIndex,int animalIndex, std::string const &path)
{
	this->setVisible(true);

	Animation3D* animation = nullptr;
	Animation3D* animation1 = nullptr;
	Animate3D* animate = nullptr;
	Animate3D* animate2 = nullptr;
	animation = Animation3D::create(path);

	if (animation)
	{
		switch (animalIndex)
		{
		case ANIMAL_GOLD:
			if (1 == posIndex)
			{
				this->setScale(1.4);
				this->setPosition3D(Vec3(60, -50, -250));
				this->setRotation3D(Vec3(90, -10, 0));
				//金鲨鱼2个动作     舞蹈：1.53333 ~ 3.53333(2.0)
				//animate = Animate3D::create(animation, 1.53333f, 2.0f);
				animate = Animate3D::create(animation, 1.5f, 2.3333f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.2);
				this->setPosition3D(Vec3(150, 20, -280));
				this->setRotation3D(Vec3(90, -10, 0));
				//金鲨鱼2个动作     休闲：0 ~ 1.2(1.2)
				animate = Animate3D::create(animation, 0.0f, 1.2f);
			}

			break;
		case ANIMAL_SILVER:
			if (1 == posIndex)
			{
				this->setScale(1.4);
				this->setPosition3D(Vec3(60, -50, -250));
				this->setRotation3D(Vec3(90, -10, 0));
				//银鲨鱼2个动作    舞蹈：1.53333 ~ 3.53333(2.0)
				//animate = Animate3D::create(animation, 1.53333f, 2.0f);
				animate = Animate3D::create(animation, 1.5f, 2.3333f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.2);
				this->setPosition3D(Vec3(150, 20, -280));
				this->setRotation3D(Vec3(90, -10, 0));
				//银鲨鱼2个动作      休闲：0 ~ 1.2(1.2)
				animate = Animate3D::create(animation, 0.0f, 1.2f);
			}
			break;
		case ANIMAL_SWALLOW:
			
			if (1 == posIndex)
			{
				this->setScale(1.4);
				this->setPosition3D(Vec3(55, -40, -250));
				//this->setRotation3D(Vec3(90, -20, 0));
				this->setRotation3D(Vec3(-180, -37, 0));
				//燕子2个动作     舞蹈：1.33333 ~ 3.33333(2.0)
				animate = Animate3D::create(animation, 1.33333f, 2.0f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.0f);
				this->setPosition3D(Vec3(150, 35, -300));
				this->setRotation3D(Vec3(-180, -25, 0));
				//燕子2个动作      休闲：0 ~ 1.0(1.0)
				animate = Animate3D::create(animation, 0.0f, 1.0f);
			
			}
			break;
		case ANIMAL_DOVE:
			if (1 == posIndex)
			{
				this->setScale(1.2);
				this->setPosition3D(Vec3(100, -40, -250));
				this->setRotation3D(Vec3(90, -20, 0));
				//鸽子2个动作     舞蹈：1.33333 ~ 3.33333(2.0)
				animate = Animate3D::create(animation, 1.33333f, 2.0f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.0f);
				this->setPosition3D(Vec3(150, 30, -280));
				this->setRotation3D(Vec3(90, -20, 0));
				//鸽子两个动作     休闲：0 ~ 1.0(1.0)
				animate = Animate3D::create(animation, 0.0f, 1.0f);
			}
			
			break;
		case ANIMAL_PEACOCK:
			if (1 == posIndex)
			{
				this->setScale(1.3);
				this->setPosition3D(Vec3(50, -50, -250));
				this->setRotation3D(Vec3(90, -20, 0));
				//孔雀2个动作     舞蹈：0 ~ 2.0(2.0)
				animate = Animate3D::create(animation, 0.0f, 2.0f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.2);
				this->setPosition3D(Vec3(150, 20, -280));
				this->setRotation3D(Vec3(92, -16, 0));
				//孔雀2个动作     休闲：2.33333 ~ 3.33333(1.0)
				animate = Animate3D::create(animation, 2.3f, 1.03333f);
			}
			
			break;
		case ANIMAL_HAWK:
			if (1 == posIndex)
			{
				this->setScale(1.5);
				this->setPosition3D(Vec3(105, -90, -250));
				this->setRotation3D(Vec3(90, -8, 0));
				//老鹰2个动作     舞蹈：1.33333 ~ 3.33333(2.0)
				animate = Animate3D::create(animation, 1.33333f, 2.0f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.2);
				this->setPosition3D(Vec3(130, 33, -280));
				this->setRotation3D(Vec3(90, -30, 3));
				//老鹰2个动作     休闲：0 ~ 1.0(1.0)
				animate = Animate3D::create(animation, 0.0f, 1.0f);
			}
		
			break;
		case ANIMAL_LION:
			if (1 == posIndex)
			{
				this->setScale(1.3);
				this->setPosition3D(Vec3(60, -52, -250));
				this->setRotation3D(Vec3(90, -20, 0));
				//新版本狮子4个动作      舞蹈：8.5 ~ 10.5(2s) 
				animate = Animate3D::create(animation, 8.5f, 2.0f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.0f);
				this->setPosition3D(Vec3(120, 10, -250));
				this->setRotation3D(Vec3(110, -20, 0));
				//新版本狮子4个动作    休闲:2.33333f ~ 4.0f(1.66667)
				animate = Animate3D::create(animation, 2.33333f, 1.66667f);
			}
			
			break;
		case ANIMAL_PANDA:
			if (1 == posIndex)
			{
				this->setScale(1.5);
				this->setPosition3D(Vec3(50, -50, -230));
				this->setRotation3D(Vec3(90, -10, 0));
				//新版本熊猫4个动作     新动作(9.1——11.83333)（0——3）()
				animate = Animate3D::create(animation,8.06666, 2.0f);
				animate2 = Animate3D::create(animation, 0.03f, 2.03f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.2);
				this->setPosition3D(Vec3(150, 15, -280));
				this->setRotation3D(Vec3(110, -30, 0));
				//新版本熊猫4个动作     休闲：1.33333 ~ 3.6(1.26667)
				//animate = Animate3D::create(animation, 1.33333f, 1.26667f);
				animate = Animate3D::create(animation, 0.0f, 2.0f);
			}
		
			break;
		case ANIMAL_MONKEY:
			if (1 == posIndex)
			{
				this->setScale(1.4);
				this->setPosition3D(Vec3(60, -45, -230));
				this->setRotation3D(Vec3(100, 0, 0));
				//新版本猴子4个动作     舞蹈：9.83334 ~ 11.83334(2.0)
				animate = Animate3D::create(animation, 9.83334f, 2.0f);

			}
			else if (2 == posIndex)
			{
				this->setScale(1.0f);
				this->setPosition3D(Vec3(130, 20, -250));
				this->setRotation3D(Vec3(110, -18, 0));
				//新版本猴子3个动作     休闲：3.33334 ~ 5.33334(2.0)
				animate = Animate3D::create(animation, 3.33332f, 2.002f);
			}
			
			break;
		case ANIMAL_RABBIT:
			if (1 == posIndex)
			{
				this->setScale(1.3);
				this->setPosition3D(Vec3(52, -30, -150));
				this->setRotation3D(Vec3(95, -15, 0));
				//新版本兔子4个动作  舞蹈： 7.5 ~ 9.5(2.0)
				animate = Animate3D::create(animation, 7.5f, 2.0f);
			}
			else if (2 == posIndex)
			{
				this->setScale(1.0f);
				this->setPosition3D(Vec3(90, 5, -150));
				//this->setRotation3D(Vec3(15, -20, -35));
				this->setRotation3D(Vec3(100, -20, 0));
				//新版本兔子4个动作  休闲：9.66667 ~ 11(1.33333)
				animate = Animate3D::create(animation, 9.66667f, 1.33333f);
			}
			
			break;
		default:
			break;
		}		
	}
	if (posIndex == 1)
	{
		if (animate2 != nullptr)
		{
			auto seq = Sequence::create(animate,DelayTime::create(0.03f), animate2, NULL);
			m_sprite3d->runAction(seq);
		}
		else
		{
			m_sprite3d->runAction(animate);
		}		
	}
	else if (posIndex == 2)
	{
		m_sprite3d->runAction(RepeatForever::create(animate));
	}
	
}

//停止动画接口
void SharkAnimal::StopAct()
{
	m_sprite3d->stopAllActions();
	this->setVisible(false);
}
