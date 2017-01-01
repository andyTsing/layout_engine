#include "paragraph_controller.h"

#include "model/text_run.h"
#include "model/character/character.h"
#include "model/character/space_character.h"
#include "model/character/basic_character.h"

namespace le {

namespace {

Character* createCharacter(char ch)
{
  switch(ch) {
    case ' ':
      return new SpaceCharacter();
    default:
      return new BasicCharacter(ch);
  }
}

}

ParagraphController::ParagraphController()
    : selected_char_(paragraph_.GetEnterRun()->GetEnterChar())
{

}

ParagraphController::~ParagraphController()
{

}

void ParagraphController::InsertText(std::string text)
{
  for( char c : text ) {
    auto character = createCharacter(c);
    selected_char_->InsertChar(character);
    paragraph_.PrintInfo();
  }
  paragraph_.GetFirstRun()->UpdateGlyph();
}

void ParagraphController::SetBold(bool bold)
{
  selected_char_->SetBold(bold);
}

void ParagraphController::SetItalic(bool italic)
{
  selected_char_->SetItalic(italic);
}

void ParagraphController::SetSize(int size)
{
  selected_char_->SetSize(size);
}


void ParagraphController::Layout()
{
  paragraph_.GetView().Layout();
}



}  // tmaxoffice
