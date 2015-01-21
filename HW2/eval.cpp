#include <string>
#include <iostream>
#include <stack>
#include <cassert>
using namespace std;

int precedence(char boolOp)
{
     switch (boolOp)
     {
          case '!': 
               return 3;
               break;
          case '&':
               return 2;
               break;
          case '|':
               return 1;
               break;
          default:
               return -1;
               break;
     }
}

bool isDigitChar(char n)
{
     return (n >= 48 && n <= 57);   //ASCII values of 0-9
}

char prevNonBlank(const string& infix, int i)              //Finds the previous non-blank character
{
     for (int j = i - 1; j >= 0; j--)
     {
          if(infix[j] == ' ')
               continue;
          else 
               return infix[j];
     }
     return '\0';                                         //Returns null char if there is no previous character
}

char nextNonBlank(const string& infix, int i)             //Finds the next non-blank character, if any
{
     for (int j = i + 1; j < infix.size(); j++)
     {
          if(infix[j] == ' ')
               continue;
          else
               return infix[j];
     }
     return '\0';
}

int evaluate(string infix, const bool values[], string& postfix, bool& result)
{
     //////////////////////////////////
     // Infix to Postfix conversion
     //////////////////////////////////
     if (infix == "")
          return 1;

     postfix = "";
     stack<char> operatorStack;
     int unpairedP = 0;                                        //Checks for stray parentheses
     int operatorCount = 0;
     int operandCount = 0;
     for(int i = 0; i < infix.size(); i++)
     {
          char chN = nextNonBlank(infix, i);
          char chP = prevNonBlank(infix, i);
          switch(infix[i])
          {
               case '(':                                                //Only operands and ! can follow an open parenthesis. Null byte means ( is the last char
                    if (!isDigitChar(chN) && chN != '!' && chN != '(' || isDigitChar(chP)) 
                         return 1;
                    operatorStack.push(infix[i]);
                    unpairedP++;
                    break;
               case ')':                                                // ) is either the last nonblank, or a binary operator follows it
                    if(unpairedP > 0)
                         unpairedP--;
                    else
                         return 1;

                    
                    if (isDigitChar(chN) || (chN != '\0' && chN != '&' && chN != '|' && chN != ')'))
                         return 1;

                    while(operatorStack.top() != '(')
                    {
                         postfix += operatorStack.top();
                         operatorStack.pop();
                    }
                    operatorStack.pop();
                    break;
               case '&':
               case '|':                
                    if((chP == '\0' || (!isDigitChar(chP) && chP != ')')) || (chN == '\0' || (!isDigitChar(chN) && (chN != '!' && chN != '('))))
                         return 1;
                    while((!operatorStack.empty() && operatorStack.top() != '(') && precedence(infix[i]) <= precedence(operatorStack.top()))
                    {
                         postfix += operatorStack.top();
                         operatorStack.pop();
                    }
                    operatorStack.push(infix[i]);
                    operatorCount++;
                    break;
               case '!':
                    if((!isDigitChar(chN) && chN != '!' && chN != '(') || chN == '\0')
                         return 1;

                    while((!operatorStack.empty() && operatorStack.top() != '(') && precedence(infix[i]) <= precedence(operatorStack.top()))
                    {
                         postfix += operatorStack.top();
                         operatorStack.pop();
                    }
                    operatorStack.push(infix[i]);
                    operatorCount++;
                    break;
               case ' ':
                    break;                                   //Ignore whitespace
               default:                                      //Either operand or invalid character
                    if (isDigitChar(infix[i]))
                    {
                         operandCount++;
                         postfix += infix[i];
                         break;
                    }
                    else                                     //Invalid character in string, return 1
                         return 1;
          }
     }
     if(unpairedP >= 1 || (operatorCount == 0 && postfix.size() > 1) || operandCount == 0)  //If the last char is an open paretheses
          return 1;

     while(!operatorStack.empty())
     {
          postfix += operatorStack.top();
          operatorStack.pop();
     }

     //////////////////////////////////
     // Postfix Expression Calculation
     //////////////////////////////////     

     stack<bool> operandStack;
     for (int i = 0; i < postfix.size(); i++)
     {
          char ch = postfix[i];
          if(isDigitChar(ch))                          //Operands
          {
               int elem = ch - 48;                     //ASCII character to integer conversion
               operandStack.push(values[elem]);
          }
          else if(ch == '&' || ch == '|')              //Binary operators
          {
               bool operand2 = operandStack.top();
               operandStack.pop();
               bool operand1 = operandStack.top();
               operandStack.pop();
               (ch == '&') ? operandStack.push(operand1 & operand2) : operandStack.push(operand1 | operand2);
          }
          else if(ch == '!')                           //Unary operator
          {
               bool operand = operandStack.top();
               operandStack.pop();
               operandStack.push(!operand);
          }

     }
     result = operandStack.top();
     return 0;
}
