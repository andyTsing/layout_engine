#include "paragraph.h"

#include "model/word.h"

#include <algorithm>
#include <iostream>

namespace le {

Paragraph::Paragraph()
    : enter_run_(this)
{}

Paragraph::~Paragraph()
{}

void Paragraph::InsertRun(Run* new_run, Run* ref_run)
{

  if( ref_run == nullptr ) {
    auto it = runs_.Append(new_run);
    new_run->SetIterator(it);
  } else {
    auto it = runs_.InsertBefore(new_run, ref_run->GetIterator());
    new_run->SetIterator(it);
  }
  new_run->SetParagraph(this);

}
void Paragraph::InsertWord(Word* new_word, Word* ref_word)
{
  
  if( ref_word == nullptr ) {
    auto it = words_.Append(new_word);
    new_word->SetIterator(it);
  } else {
    auto it = words_.InsertBefore(new_word, ref_word->GetIterator());
    new_word->SetIterator(it);
  }
  new_word->SetParagraph(this);
}


void Paragraph::PrintWord()
{
  auto word = words_.GetFirstNode();

  for( ; word ; word = word->GetNextWord() )
  {
    auto ch = word->GetFirstCharacter();
    std::string str;
    for( ; ch ; ch = ch->GetNextCharacter() )
      str += ch->GetChar();

    std::cout<< "["<<str << "] ";
    
  }

  std::cout<< std::endl;
}


void Paragraph::PrintRun()
{
  auto run = runs_.GetFirstNode();

  for( ; run ; run = run->GetNextRun() )
  {
    auto ch = run->GetFirstCharacter();
    std::string str;
    for( ; ch ; ch = ch->GetNextCharacter() )
      str += ch->GetChar();

    std::cout<< "["<<str << "] ";
    
  }

  std::cout<< std::endl;
}




}  // le
