#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "../ALL/comm.h"
#include <cstdlib>
#include "../ALL/gzstream.h"
#include <zlib.h>
#include <stdio.h>
#include "../ALL/kseq.h"

using namespace std;

int  print_AusageA35()
{
	cout <<""
		"\n"
		"\tUsage: extractP -i <in.fa>  -o <out.fa> -s <str>\n"
		"\n"
		"\t\t-i    <str>    input FASTA\n"
		"\t\t-o     <str>   output file, default [STDOUT]\n"
		"\n"
		"\t\t-s    <str>    extract one sequence by specifying sequence ID\n"
		"\t\t-l    <str>    extract multiple sequences by specifying sequence IDs in a list\n"
		"\t\t-r    <str>    remove sequence(s) by specifying sequence ID(s) in a list\n"
		"\t\t-m    <str>    extract sequences whose IDs match the pattern\n"
		"\t\t-u    <str>    extract sequences whose IDs don't match the pattern\n"
		"\t\t               don't use with four para above together\n"
		"\n"
		"\t\t-h             show more details for help\n" 
		"\n";
	return 1;
}

void More_HelpFA_7 ()
{
cout<<"\n\
\n\
\t\t1.  extractp -i <in.fa> -o AAA -s 'seq1'\n\
\t\tthis will extract the sequence whose ID is 'seq1' from FASTA and output the result in a compressed file named AAA.gz. \n\
\n\
\t\t2.  extractp -i <in.fa> -o AAA -l seq_list\n\
\t\tthis will extract the sequence s listed in seq_list from FASTA and output the result in a compressed file named AAA.gz. For example, if user would like to extract three sequences (seqA, seqB and seqC) from FASTA, the seq_list should be formatted as: \n\
\t\tseqA\n\
\t\tseqB\n\
\t\tseqC\n\
\n\
\t\t3.  extractp -i <in.fa> -o AAA -r seq_list\n\
\t\tthis will remove the sequences listed in seq_list from FASTA and output sequences left to a compressed file named AAA.gz. For example, if user would like to remove three sequences (seqA, seqB and seqC) from FASTA, the seq_list should be formatted as: \n\
\t\tseqA\n\
\t\tseqB\n\
\t\tseqC\n\
\n\
\t\t4.  extractp -i <in.fa> -o AAA -m 'XY'\n\
\t\tthis will extract the sequences whose IDs containing the pattern 'XY' from FASTA and output the result in a compressed file named AAA.gz. \n\
\n\
\t\t5.  extractp -i <in.fa> -o AAA -u 'XY'\n\
\t\tthis will extract the sequences whose IDs don't contain the patter'XY' from FASTA and output the result in a compressed file named AAA.gz\n\
\n\
\n";
}


int parse_AcmdA35 (int argc, char **argv , ParaClass  * paraA35)
{
	if (argc <=1 ) {print_AusageA35();return 0;}
	for(int i = 1; i < argc ; i++)
	{
		if(argv[i][0] != '-')
		{
			cerr << "command option error! please check." << endl;
			return 0;
		}
		string flag=argv[i] ;
		flag=replace_all(flag,"-","");

		if (flag  == "InFa" ||  flag  == "i" )
		{
			if(i + 1 == argc) { LogLackArg(flag); return 0;}
			i++;
			paraA35->InPut1=argv[i];
		}
		else if (flag  ==  "OutPut" ||  flag  == "o")
		{
			if(i + 1 == argc) { LogLackArg(flag); return 0;}
			i++;
			paraA35->OutPut1=argv[i];
		}
		else if (flag  ==  "GetIDList"  ||  flag  == "l")
		{
			if(i + 1 == argc) { LogLackArg(flag); return 0;}
			i++;
			paraA35->InStr2=argv[i];
		}
		else if (flag  ==  "RmIDList" ||  flag  == "r")
		{
			if(i + 1 == argc) { LogLackArg(flag); return 0;}
			i++;
			paraA35->InStr3=argv[i];
		}
		else if (flag  ==  "GetID"  ||  flag  == "s")
		{
			if(i + 1 == argc) { LogLackArg(flag); return 0;}
			i++;
			paraA35->InStr1=argv[i];
		}
		else if (flag  ==  "Pattern"  ||  flag  == "m")
		{ 
			if(i + 1 == argc) { LogLackArg(flag); return 0;}
			i++;
			paraA35->InPut2=argv[i];
		}
		else if (flag  ==  "UnPattern"  ||  flag  == "u")
		{ 
			if(i + 1 == argc) { LogLackArg(flag); return 0;}
			i++;
			paraA35->OutPut2=argv[i];
		}
		else if (flag  == "help"  ||  flag  == "h")
		{
			More_HelpFA_7();return 0;
		}
		else
		{
			cerr << "UnKnow argument -"<<flag<<endl;
			return 0;
		}
	}
	
	bool Para1=( (paraA35->InPut2).empty() && (paraA35->InStr1).empty() &&  (paraA35->OutPut2).empty() && (paraA35->InStr2).empty() ) ;

	if  ( (Para1)  &&  ((paraA35->InStr3).empty()) )
	{
		cerr<< "lack argument for the must"<<endl ;
		return 0;
	}
	else if  (  (paraA35->InPut1).empty()  )
	{
		cerr<< "lack argument for the must"<<endl ;
		return 0;
	}
	else if (  (!Para1)  &&   (!(paraA35->InStr3).empty()) )
	{
		cerr<< "-RmIDList can not use with the other four Para"<<endl ;
		return 0;
	}
	if  (!(paraA35->OutPut1).empty() )
	{        
		(paraA35->OutPut1)=add_Asuffix(paraA35->OutPut1);
	}

	return 1 ;

}


int FA_macth_main(int argc, char *argv[])
	//int main(int argc, char *argv[])
{
	ParaClass * paraA35 = new ParaClass;
	if( parse_AcmdA35(argc, argv, paraA35 )==0)
	{
		delete  paraA35 ;
		return 1 ;
	}

	igzstream  IN ((paraA35->InPut1).c_str(),ifstream::in);
	if(!IN.good())
	{
		cerr << "open InputFile error: "<<(paraA35->InPut1)<<endl;
		delete  paraA35 ; return 1;
	}

	map <string,bool> IDHash ;
	bool  FlagRm_or_Get=true ;
	if (!((paraA35->InStr1).empty()))
	{
		IDHash.insert(map <string,bool>:: value_type((paraA35->InStr1),true));
	}
	if (!((paraA35->InStr2).empty()))
	{
		igzstream  INID ((paraA35->InStr2).c_str(),ifstream::in);
		if(!INID.good())
		{
			cerr << "open InputFile error: "<<(paraA35->InStr2)<<endl;
			delete  paraA35 ; return 1;
		}		
		while(!INID.eof())
		{
			string line,ID;
			getline(INID, line);
			if (line.empty())
			{
				continue;
			}
			istringstream isone (line,istringstream::in);
			isone>>ID;
			IDHash.insert(map <string,bool>:: value_type(ID,true));
		}
		INID.close();
	}

	if (!((paraA35->InStr3).empty()))
	{
		igzstream  INIDRM ((paraA35->InStr3).c_str(),ifstream::in);
		if(!INIDRM.good())
		{
			cerr << "open InputFile error: "<<(paraA35->InStr3)<<endl;
			delete  paraA35 ; return 1;
		}
		while(!INIDRM.eof())
		{
			string line,ID;
			getline(INIDRM, line);
			if (line.empty())
			{
				continue;
			}
			istringstream isone (line,istringstream::in);
			isone>>ID;
			IDHash.insert(map <string,bool>:: value_type(ID,true));
		}
		INIDRM.close();
		FlagRm_or_Get=false ;
	}


	if ( (paraA35->OutPut1).empty() )
	{
		string seqAA ;
		getline(IN, seqAA, '>');
		while(!IN.eof())
		{
			string chr_line , chr_name;
			getline(IN, chr_line , '\n') ;
			getline(IN, seqAA , '>') ;
			istringstream isone (chr_line,istringstream::in);
			isone>>chr_name ;
			map <string, bool> :: iterator it ;
			if ((!IDHash.empty()))
			{
				it=IDHash.find(chr_name);
				if ( (FlagRm_or_Get)  ==  (it!=IDHash.end())) { cout<<">"<<chr_line<<"\n"<<seqAA; it->second=false  ;}
			}
			if ( (!(paraA35->InPut2).empty()) && (chr_name.find((paraA35->InPut2))!=string::npos) )
			{
				cout<<">"<<chr_line<<"\n"<<seqAA;
			}
			else if (  (!(paraA35->OutPut2).empty()) && (chr_name.find((paraA35->OutPut2))==string::npos) )
			{
				cout<<">"<<chr_line<<"\n"<<seqAA;
			}
		}
	}
	else
	{
		ogzstream OUT ((paraA35->OutPut1).c_str());
		if (OUT.fail())
		{
			cerr << "open OUT File error: "<<(paraA35->OutPut1)<<endl;
			delete  paraA35 ; return 1;
		}
		string seqAA ;
		getline(IN, seqAA, '>');
		while(!IN.eof())
		{
			string chr_line , chr_name;
			getline(IN, chr_line , '\n') ;
			getline(IN, seqAA , '>') ;
			istringstream isone (chr_line,istringstream::in);
			isone>>chr_name ;
			map <string, bool> :: iterator it ;
			if ((!IDHash.empty()))
			{
				it=IDHash.find(chr_name);
				if (   (FlagRm_or_Get) == (it!=IDHash.end())) { OUT<<">"<<chr_line<<"\n"<<seqAA;  it->second=false ;}
			}

			if ((!(paraA35->InPut2).empty()) && (chr_name.find((paraA35->InPut2))!=string::npos) )
			{
				OUT<<">"<<chr_line<<"\n"<<seqAA;
			}
			else if ((!(paraA35->OutPut2).empty()) && (chr_name.find((paraA35->OutPut2))==string::npos) )
			{
				OUT<<">"<<chr_line<<"\n"<<seqAA;
			}
		}
		OUT.close();
	}

	if  (FlagRm_or_Get)
	{
		map <string, bool> :: iterator it ;
		for (it=IDHash.begin();  it!=IDHash.end() ; it++)
		{
			if (it->second)
			{
				cerr<<"can't_find_ID\t"<<it->first<<endl;
			}
		}

	}

	delete paraA35 ;
	return 0;
}

///////// swimming in the sky and flying in the sea ////////////

