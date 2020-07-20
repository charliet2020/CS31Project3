#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
using namespace std;

bool hasCorrectSyntax(string song)
{
	if (song == "") //zero beats and empty string
		return true;
	else if (song[song.size() - 1] != '/')
		return false; 
	else
	{
		for (int k = 0; k != song.size(); k++) // check characters other than given colors, digits, and slash
		{
			if (song[k] != '/' && song[k] != 'g' && song[k] != 'r' && song[k] != 'y' && song[k] != 'b' && song[k] != 'o'
				&& song[k] != 'G' && song[k] != 'R' && song[k] != 'Y' && song[k] != 'B' && song[k] != 'O' && !isdigit(song[k]))
				return false;
			else
			{
					if (song[k] == 'g' || song[k] == 'r' || song[k] == 'y' || song[k] == 'b' || song[k] == 'o' ||
						song[k] == 'G' || song[k] == 'R' || song[k] == 'Y' || song[k] == 'B' || song[k] == 'O')
					{
						if (((k + 1) != song.size()) && isdigit(song[k + 1])) //one digit. Ex: g3/. //signed mismatch. Should I delete less than song size? ((k + 1) < song.size())
						{ // change < to !=
							continue;

						}
						else
						{
							if (((k + 1) != song.size()) && song[k + 1] == '/') // checks for /gg/
								continue;
							else
								return false;
						}
					}
					else // for songs that start with arbitrary '/' in front
					{
						if (song[k] == '/') // Ex: "///"
						{
							if (((k + 1) != song.size()) && (song[k + 1] == '/' || song[k + 1] == 'g' || song[k + 1] == 'r' || song[k + 1] == 'y' || song[k + 1] == 'b' ||
								song[k + 1] == 'o' || song[k + 1] == 'G' || song[k + 1] == 'R' || song[k + 1] == 'Y' || song[k + 1] == 'B' || song[k + 1] == 'O')) //only alpha and / may follow
							{
								//if ((k + 1) != song.size())
								continue; //go back to for loop; begin next iteration. 

							}
							else
							{
								if ((k + 1) == song.size()) //Must mean last character is a '/'
									return true;
								else						//tests for cases like "//3/". Here, second slash has 3 after, which fails second condition in parenthesis.
									return false; //if the stuff in giant parenthesis fails 
							}
						}
						else //if digits are in string
						{
							if (isdigit(song[k]) && (k - 1) < 0) //if integer is started off. Ex: 3g/, 3, 02/,  
								return false;
							else
							{
								if (isdigit(song[k])) //after first digit, which should have letter before, either '/' or second digit should follow
								{
									if (isalpha(song[k - 1]) && (song[k + 1] == '/' || isdigit(song[k + 1]))) // Letters other than color already taken care of. cannot have 3 digits in a row. Ex: g303/
									{
										continue;
									}
									else
									{
										if (isdigit(song[k - 1]) && song[k + 1] == '/') // if digit before digit, after second digit must have '/'
										{
											continue;
										}
										else // g3g/ is wrong
											return false;	//g303/ gets 0, g30/ gets 1. Pass. 
									}
								}
							}
						}
					}
				
			}
		}
		return true;
	}
}
int translateSong(string song, string& instructions, int& badBeat)
{
	string s = ""; // will use to set equal to instructions if song is translatable
	if (hasCorrectSyntax(song))
	{
		for (int k = 0; k != song.size(); k++) // Note: if multiple errors, will only report most left.
		{
			if (isdigit(song[k])) 
			{
				if (song[k] == '0')
				{
					if (song[k + 1] == '0' || song[k + 1] == '1') //sustained beat of 00 and 01 not allowed
					{
						badBeat = k + s.size(); 
						return 2;
					}
					else
						if (isalpha(song[k - 1]) && song[k + 1] == '/') // ensures g0/ fails
						{
							badBeat = k + s.size();
							return 2;
						}
						else
						{
							if (isalpha(song[k - 1]) && isdigit(song[k + 1])) //assumes k+1 is greater than 1; 1st "if" below k=0 takes care of otherwise. Ex: g03/// 
							{
								int i = (song[k] - '0')*10 + (song[k+1] - '0');
								if ((k + 1 + i) < song.size())
								{
									int count = 0;
									for (int j = k + 2; song[j] == '/'; j++)
										count++;
									if (count >= i) // translatable now. Ex: g05///////
									{
										for (int n = 0; n < i; n++)
										{
											s += toupper(song[k - 1]);
										}
										k += i+1;  
										continue;
									}
									else // big song, but not enough immediate '/', Ex: 
									{
										badBeat = count + 1 + s.size();
										return 3;
									}
								}
								else 
								{
									for (int j = k + 2; j != song.size(); j++)
										if (song[j] != '/') //if digit is large, but during sustained note, there's another beat. Ex: g09/g/
										{
											badBeat = j-2 + s.size();
											return 3;
										}
									 
									{//if beat ends prematurely (less dashes than number) 
										int count = 0;
										for (int j = k; j != song.size(); j++) //will count total '/' that follow.
										{
											if (song[j] == '/')
												count++;
										}
										badBeat = count + 1 + s.size();
										return 4;
									}
								}
							}
						}
				}
				else
				{
					if (song[k] == '1')
					{
						if (isalpha(song[k - 1]) && song[k + 1] == '/') //ensures g1/ fails
						{
							badBeat = k + s.size();
							return 2;
						}
						else //k-1 will always be letter; otherwise, bad syntax
						{
							if (isdigit(song[k + 1]))
							{
								int i = (song[k] - '0') * 10 + (song[k + 1] - '0');
								if ((k + 1 + i) < song.size())
								{ 
									int count = 0;
									for (int j = k + 2; song[j]=='/'; j++)
										count++;
									if (count >= i) // translatable now, will add later
									{
										for (int n = 0; n < i; n++)
										{
											s += toupper(song[k - 1]);
										}
										k += i + 1;
										continue;
									}
									else // big song, but not enough immediate '/'. Ex: g11/////g///////////////
									{
										badBeat = count + 1 + s.size();
										return 3;
									}
								}
								else
								{
									for (int j = k + 2; j != song.size(); j++) //small song with less immediate '/'. 
									{
										if (song[j] != '/')
											{
												badBeat = j - 2 + s.size();
												return 3;
											}
									}
									//if beat ends prematurely (less dashes than number)
										int count = 0;
										for (int j = k; j != song.size(); j++) //will count total / behind.
										{
											if (song[j] == '/')
												count++;
										}
										badBeat = count + 1 + s.size();
										return 4;
									
								}
							}
						}
					}
					else // non 0-1 cases. (more general). 
					{
						if (isdigit(song[k + 1]))
						{
							int i = (song[k] - '0') * 10 + (song[k + 1] - '0');
							if ((k + 1 + i) < song.size())
							{ //will add more
								int count = 0;
								for (int j = k + 2; song[j] == '/'; j++)
									count++;
								if (count >= i) //translatable now
								{
									for (int n = 0; n < i; n++)
									{
										s += toupper(song[k - 1]);
									}
									k += i + 1;
									continue;
								}
								else //big song, but not enough immediate '/'
								{
									badBeat = count + 1 + s.size();
									return 3;
								}
							}
							else
							{
								for (int j = k+2;j!=song.size();j++)
									if (song[j] != '/') //large digit, but sustained and another beat
									{
										badBeat = j - 2 + s.size(); //account for jumping over characters to find '/'
										return 3;
									}
								
								int count = 0; //for premature cases like g20/
								for (int j = k; j != song.size(); j++) //will count total / behind.
								{
									if (song[j] == '/')
										count++;
								}
								badBeat = count + 1 + s.size();
								return 4;
							}
						}
						else //must mean slash follows and value is less than 10
						{
							int i = (song[k] - '0');
							if ((k + i) < song.size())
							{
								int count = 0;
								for (int m = k + 1; song[m] == '/'; m++)
									count++;
								if (count >= i) //translatable now
								{
									for (int n = 0; n < i; n++)
									{
										s += toupper(song[k - 1]);
									}
									k += i ;
									continue;
								}
								else // large song but not enough immediate '/'
								{
									badBeat = count + 1 + s.size();
									return 3;
								}
							}
							else
							{
								for (int m=k+1;m!=song.size();m++) // Ex: g5/g/
									if (song[m] != '/')
									{
										badBeat = m - 1 + s.size();
										return 3;
									}
								int count = 0; //premature cases like g5//
								for (int j = k; j != song.size(); j++)
								{
									if (song[j] == '/')
										count++;
								}
								badBeat = count + 1 + s.size();
								return 4;
							}
						}
					}
				}

			}
			if (song[k] == '/')
			{
				if ((k - 1) < 0) // in case first char is /
					s += "x";
				if ((k - 1) >= 0 && song[k - 1] == '/')
					s += "x";	
			}
			if (isalpha(song[k]))
			{
				if (song[k + 1] == '/') //Ex: g/g/ should print gg
					s += tolower(song[k]); //will just move to next iteration if not.
			}
		}
		instructions = s;
		return 0;
	}
	else
		return 1;
}

int main()
{
	cout << "Enter song: " ;
	string song;
	getline(cin, song);
	string instructions;
	int badBeat;
	cout << translateSong(song, instructions, badBeat) << endl;
	cout << badBeat << endl;
	cout << instructions << endl;
}
// try adding s.size() to the return 2-4 to account for valid beats in front of error. 


