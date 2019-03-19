#include "OneOfRecord.h"


#define      ANIAMLLOGO      "animal"
OneOfRecord::OneOfRecord() : logo(nullptr)
{

}

OneOfRecord::~OneOfRecord()
{
}

void OneOfRecord::setContent()
{
	logo = dynamic_cast<ImageView*>(_seekNodeByName(m_pNode, ANIAMLLOGO));
	CC_ASSERT(logo != NULL);

}

void OneOfRecord::setImage(int index)
{
	switch (index)
	{
	case animalKind::ANIMAL_GOLD:
		logo->loadTexture(LOGO_GOLD, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_SILVER:
		logo->loadTexture(LOGO_SILVER, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_SWALLOW+1:
		logo->loadTexture(LOGO_SWALLOW, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_DOVE+1:
		logo->loadTexture(LOGO_DOVE, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_PEACOCK+1:
		logo->loadTexture(LOGO_PEACOCK, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_HAWK+1:
		logo->loadTexture(LOGO_HAWK, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_LION + 1:
		logo->loadTexture(LOGO_LION, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_PANDA + 1:
		logo->loadTexture(LOGO_PANDA, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_MONKEY + 1:
		logo->loadTexture(LOGO_MONKEY, TextureResType::PLIST);
		break;
	case animalKind::ANIMAL_RABBIT + 1:
		logo->loadTexture(LOGO_RABBIT, TextureResType::PLIST);
		break;

	}
}


