#include <iostream>
#include <array>
using namespace std;
class passWordChecker{
    public:
        string password;
        string secondPassword;
		array <int, 5> charCount;
		int passwordLength;
		
        passWordChecker(string pwd, string pwdtwo){
            password = pwd;
            secondPassword = pwdtwo;
            passwordLength = 0;
        }
        passWordChecker(){};
        void displayRules(){
            cout<<"Hello! Please create a new password."<<endl;
            cout<<"The set of password rules is given below:"<<endl<<endl;
            cout<<"   1.) Password length must be 8 to 32 characters"<<endl;
            cout<<"   2.) It must contain at least one numerical, i.e. 0, 1, 2..."<<endl;
            cout<<"   3.) It must contain at least one upper case letter, i.e. A, B, C..."<<endl;
            cout<<"   4.) It must contain at least one lower case letter, i.e. a, b, c..."<<endl;
            cout<<"   5.) It must contain at least one of the special characters within the"<<endl<< +
                    "       set: { # $ * ( ) % & ^} a total of eight (8) special characters,"<<endl<< +
                    "       and no other special characters are allowed."<<endl;
            cout <<endl;
        }
        void askUser4passwd(){
            cout<<"Please enter your password:"<<endl;
        }
        void displaySucess(){
            cout<<"Your password will be updated in 2 minutes."<<endl;
        }
        void displayFail(){
            cout<<"Your password failed one or more password rules."<<endl<<endl;
        }
        void displayMatchFail(){
            cout<<"Match fail..."<<endl;
        }
        int checkOneChar(char sample){
            int converted;
            int inx;
            converted = int(sample);
            if (converted >=48 && converted <=57){
                inx = 1;
            }
            else if (converted >=97 && converted <=122){
                inx = 2;
            }
            else if (converted >=65 && converted <=90){
                inx = 3;
            }
            else if ((converted >= 35 && converted <=38) || (converted >=40 && converted <= 42) || converted == 94){
                inx = 4;
            }
            else{
                inx = 0;
            }
            return inx;
        }
        int checkRules(){
            if ((charCount[0]==0) && (charCount[1] >0) && (charCount[2] >0) && (charCount[3] >0) && (charCount[4] > 0)) return 1;
            else return 0;
        }
        int matching(string s1, string s2){
            if (s1 == s2) return 1;
            else return 0;
        }
};
int main() {
    int i; int index = 0; int validYesNo = 0;
    int matchYesNo = 0;
    passWordChecker *pwc;
    pwc = new passWordChecker();

    //STEP 13
    while (matchYesNo == 0) {

        //STEP 9
        while (validYesNo == 0) {

            //STEP 1
            pwc->displayRules();

            //STEP 2 & 3
            while ((pwc->passwordLength > 32) || (pwc->passwordLength < 8)) {
                pwc->askUser4passwd();
                cin >> pwc->password;
                pwc->passwordLength = pwc->password.length();

                if ((pwc->passwordLength > 32) || (pwc->passwordLength < 8)){
                    cout <<"ERROR: Password must be between 8 & 32 characters."<<endl;
                }
            }

            //STEP 4
            i = 0;

            //STEP 7
            while (i < pwc->password.length()) {

                //STEP 5
                index = pwc->checkOneChar(pwc->password[i]);
                //cout<<i <<": "<<index <<endl;
                if (index >=0 && index <=4){
                    pwc->charCount[index]++;
                }

                //STEP 6
		i++;
            }
            cout<<endl;
            
	    //STEP 8
            validYesNo = pwc->checkRules();
            if (validYesNo == 0) {
                pwc->displayFail();
                pwc->password = "";
                pwc->passwordLength = 0;
                (pwc->charCount).fill(0);
            }
        }

        //STEP 10
        cout <<"Please re-type your password:"<<endl;

        //STEP 11
        cin >> pwc->secondPassword;

        //STEP 12
        matchYesNo = pwc->matching(pwc->password, pwc->secondPassword);
        if (matchYesNo == 0) {
            pwc->displayMatchFail();
            pwc->password = "";
            pwc->passwordLength = 0;
            (pwc->charCount).fill(0);
            validYesNo = 0;
        }
    }

    //STEP 14
    if (matchYesNo == 1) pwc->displaySucess();
    return 0;
}
