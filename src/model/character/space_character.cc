#include "space_character.h"

#include "model/word.h"

namespace le {

SpaceCharacter::SpaceCharacter()
    : Character(' '),
      view_(*this)
{}

SpaceCharacter::~SpaceCharacter()
{}

void SpaceCharacter::InsertChar(Character* character)
{
  if( character->GetChar() == ' ' ) {
    GetWord()->InsertCharacter( character, this );
  }
  else {
    if( GetPrevWordCharacter()->GetChar() == ' ' ) {
      // split
      
    }
    else {
      // no split
      GetWord()->InsertCharacter( character, this );
    }
  }

}




}  // le
