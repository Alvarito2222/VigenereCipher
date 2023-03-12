//Alvaro Leon Parra    
//Project2 - VigenereCipher
//Encryption/Decryption

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

const int ALPHABET_SIZE = 26;

void printFrequency(const string& text) 
{ 															// Takes a string as input and prints the frequency of each letter in the string.
    int frequency[ALPHABET_SIZE] = {0};

    for (char c : text) {
        if (isalpha(c)) {
            ++frequency[tolower(c) - 'a'];
        }
    }

    cout << "Letter Frequency:" << endl;
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        char c = 'a' + i;
        cout << c << ": " << frequency[i] << endl;
    }
}


double indexOfCoincidence(const string& text) 	
{															 // Takes a string as input and returns the index of coincidence (a measure of how similar the string is to a random sequence of characters) of the string.
    int total = 0;
    int frequencies[26] = {0};
    for (char c : text) {
        if (isalpha(c)) {
            ++frequencies[tolower(c) - 'a'];
            ++total;
        }
    }
    double sum = 0.0;
    for (int i = 0; i < 26; ++i) {
        double f = frequencies[i];
        sum += f * (f - 1);
    }
    double IOC = sum / (total * (total - 1));
    return IOC;
}



void guessKeyLength(const string& text, int& keyLength) 
{															//Takes a string and an integer reference as input and guesses the length of the key used to encrypt the string.
    double maxIOC = 0;
    int bestKeyLength = 1;

    for (int i = 1; i <= text.size() / 2; ++i) {
        double sumIOC = 0;
        for (int j = 0; j < i; ++j) {
            string subtext;
            for (int k = j; k < text.size(); k += i) {
                subtext += text[k];
            }
            double IOC = indexOfCoincidence(subtext);
            sumIOC += IOC;
        }
        double avgIOC = sumIOC / i;
        double kappa = (0.027 * text.size()) / (avgIOC * (text.size() - 1) + 0.065 - 0.038 * text.size());
        if (kappa > maxIOC) {
            maxIOC = kappa;
            bestKeyLength = i;
        }
    }
    keyLength = bestKeyLength;
}


int findShift(const string& text) 
{ 														// takes a string as input and returns the shift used to encrypt the string using the Vigenere cipher.
    int frequency[26] = {0};
    for (char c : text) {
        ++frequency[c - 'A'];
    }
    int maxFrequency = 0;
    int shift = 0;
    for (int i = 0; i < 26; ++i) {
        if (frequency[i] > maxFrequency) {
            maxFrequency = frequency[i];
            shift = i;
        }
    }
    return (26 - shift + 4) % 26;
}

string guessKey(const string& text, int keyLength) 
{ 														// Takes a string and an integer as input and guesses the key used to encrypt the string. 																
    string key;											//It divides the original string into sub-strings encrypted with the same letter of the key and calculates the shift for each sub-string.
    string subText;										// It then uses the shift values to guess the corresponding letters of the key.
    for (int i = 0; i < keyLength; ++i) {
        for (int j = i; j < text.size(); j += keyLength) {
            subText += text[j];
        }
        int shift = findShift(subText);
        char letter = (shift + 'A' - 4) % 26 + 'A';
        key += letter;
    }
    return key;
}





string encrypt(string plaintext, string keyword)
{
    string ciphertext = "";

    for (int i = 0, j = 0; i < plaintext.length(); i++)
    {
        char c = plaintext[i];

        if (isalpha(c))										// Check if the character is a letter
        {
            												// Shift the character by the corresponding keyword letter
            ciphertext += (char)((toupper(c) + toupper(keyword[j]) - 2 * 'A') % 26 + 'A');
            j = (j + 1) % keyword.length();
        }
    }

    return ciphertext;
}

string decrypt(string ciphertext, string keyword)
{
    string plaintext = "";

    for (int i = 0, j = 0; i < ciphertext.length(); i++)
    {
        char c = ciphertext[i];

        												// Check if the character is a letter
        if (isalpha(c))
        {
            											// Shift the character back by the corresponding keyword letter
            plaintext += (char)((toupper(c) - toupper(keyword[j]) + 26) % 26 + 'A');
            j = (j + 1) % keyword.length();
        }
    }

    return plaintext;
}

int main()
{
    string keyword, guessedkeyword, plaintext, ciphertext;
    string fileName;
    string outputFileName;
    string ciphertext1;
    string encryptedFile;
    int option;
    char c;
    bool option2;
    
	do{
		option2 = false;
    	cout << "Enter the keyword for the Vigenere cipher: ";
    	cin >> keyword;

    	for (int i = 0; i < keyword.length(); i++) 
		{															// Iterate through the string one letter at a time.
        	c = keyword.at(i);         									// Get a char from string
        	if (! ( ( c >= 'a' && c <= 'z' ) || ( c >= 'A' && c <= 'Z' ) ) ) 
				{ 																// if it's NOT within these bounds, then it's not a character

             	cout << "Error! You must enter Letters." << endl;
             	option2 = true;
        	}
        	
     	}
     	
     }while(option2);

    cout << "Enter the name of the plaintext file: ";
    cin >> fileName;

    ifstream inputFile(fileName);
    if (inputFile.is_open())
    {
        string line;
        while (getline(inputFile, line))
        {
            plaintext += line;
        }
        inputFile.close();
    }
    else
    {
        cout << "Unable to open file." << endl;
        return 1;
    }

    
	do{
		ciphertext = encrypt(plaintext, keyword);
		
    	cout << endl << "What would you like to do?[1][2][3]" << endl;
    	cout << "1. Display the ciphertext" << endl;
    	cout << "2. Write the results to a specified text file" << endl;
    	cout << "3. Decrypt a file" << endl;
    	cout << "4. EXIT" << endl;
    	cout << "Enter your choice: ";
    	cin >> option;
    
		if(option == 1 || option == 2 || option == 3 || option ==4 )
		{

    	switch (option)
    	{
        	case 1:
        	    cout << endl << "Ciphertext: ";
            	for (int i = 0; i < ciphertext.length(); i++) 
				{
        			cout << ciphertext[i];
        			if ((i + 1 ) % 5 == 0) 										// After 5 letters , it does a space 
					{
            		cout << " ";
        			}		
    			}
    			cout << endl;
            	break;
        	case 2:
        		{
				
        		cout <<endl << "Enter the name of the output file: ";
            	cin >> outputFileName;
            	ofstream outputFile(outputFileName);
            	if (outputFile.is_open())
            	{
                	outputFile << ciphertext;
                	outputFile.close();
                	cout << "Data written to file successfully." << endl;
                	break;
            	}
				else
				{
					cout << "Unable to open file." << endl;
					break;
				}
				break;
			}
        	case 3:
            	
            
    			cout << "Enter the name of the encrypted file: ";
    			cin >> encryptedFile;

    			ifstream input2File(encryptedFile);
    			if (input2File.is_open())
    			{
        			string line2;
        			while (getline(input2File, line2))
        			{
            			ciphertext1 += line2;
        			}
        			input2File.close();
    			}
    			else
    			{
        			cout << "Unable to open file." << endl;
        			break;
    			}
    				printFrequency(ciphertext1);
    				cout << "Index of Coincidence: " << indexOfCoincidence(ciphertext1) << endl;
    				int keyLength = 0;
    				guessKeyLength(ciphertext1, keyLength);
    				cout << "Predicted Key Length: " << keyLength << endl;
    				string key = guessKey(ciphertext1, keyLength);
    				cout << "Predicted Key: " << key << endl;
    				decrypt(ciphertext1, key);
    				int userInput;
    				do
					{
						cout << "\n1. Yes\n2. No\n3. Enter your own keyword\n";
						cout << "\nDo you want to use the key lenght of " << keyLength << "?[1][2][3]: ";
						cin >> userInput;

						if (userInput == 1)
						{
						
						string key = guessKey(ciphertext1, keyLength);
    					cout << "Predicted Key: " << key << endl;
    					cout << endl;
						cout << "\nThe plain text is: ";
    					cout << decrypt(ciphertext1, key) << endl;;
			
						}
						else if(userInput == 2)
						{
							cout << "\n Enter a new guess key length: " ;
							cin >> keyLength;
							
							string key1 = guessKey(ciphertext1, keyLength);
    						cout << "Predicted Key: " << key1 << endl;
    						
							
						}
						else
						{
							userInput = 3;
						}
						
					} while (userInput != 3);
					
					
						string userKeyWord;
						cout << " Enter the keyword: ";
						cin >> userKeyWord;
						cout << endl;
						cout << "\nThe plain text is: ";
						cout << decrypt(ciphertext1, userKeyWord) << endl;;
						
				
    			
            		break;

	
	}	
}
	else if(option !=1 && option !=2 && option !=3 && option !=4)						// if something that is not [6][5][4][3][2][1] is inserted , program will say try again and clear input.
		{
			system("cls");
			cin.clear();                   
            cin.ignore(1000, '\n');         
			cout<<"Try again [1][2][3][4]";
			cout<<"\n";
		}
	

	}while(option !=4 );
	
	cout << endl << "!!SEE YOU SOON!!" << endl;
	return 0;
}
