#include<deque>
#ifndef _TTAG
#define _TTAG
#include<vector>
#include<assert.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include <cmath>
#define FIFO_SIZE	100
#define ANALOG_SIZE	257
#define COIN_SIZE	512
#define MAX30		1073741824L
#include<iostream>
#include<fstream>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdint.h>
#define INDEPENDENT
#ifdef INDEPENDENT

using namespace std;

class queueElement {
	public:
		queueElement():data(NULL),timeStamp(0),freq(0),wideOutput(false),
		alatch(0),sn(0),isDelimiter(false){ };
		queueElement(const queueElement& qe);
		~queueElement(){ if(data) delete data;}
		void setData( struct TTAG_DATA *t);
		void setFreq(double f){ freq = f; };
		void process();
		int al(){ return alatch; };
		unsigned id(){ return sn; };
		long double getTimeStamp(){ return timeStamp; };
		int getAlatch(){ return alatch; };
		void setDelimiter(){ isDelimiter = true; return; };
		void report();

	private:
		bool wideOutput;
		struct TTAG_DATA *data;
		long double timeStamp;
		double freq;
		int alatch;
		unsigned int sn;
		bool isDelimiter;
};

class myQueue: public deque<queueElement*> {
	public:
		myQueue();
		int insert(queueElement *el);
		void eraseLast(int flag = 0);
		double getTicksSize(){ return TicksSize; };
		double getTimeSize(){ return TimeSize; };
	private:
		long double TicksSize;
		long double TimeSize;
};

class EQ {
	public:
		EQ();
		~EQ(){ dumpQueue(); return; };
		void insert(queueElement *el);
		void dumpQueue();

	private:
		const int lengthLimit;
		double calculateFrequency();
		myQueue past;
		myQueue future;
};



struct TTAG_DATA
{
            uint32_t id;
            int32_t deltaTime;
            uint32_t pcTTTime;
            uint32_t pcTRAIMTime;
            uint32_t pcTDTime;
            int32_t month, day, year;
            int32_t hours, minutes, seconds;
            int32_t sawTooth;
            int32_t blatchCount;
            int32_t analogCount;
            int32_t coinCount;
            int32_t alatch;
            int32_t temperature[4];
            int32_t hv[3];
            int32_t blatch[FIFO_SIZE];
            int32_t analog[ANALOG_SIZE];
            int32_t coin[COIN_SIZE];
};

#else

struct TTAG_DATA
{
            unsigned long id;
            long deltaTime;
            unsigned long pcTTTime;
            unsigned long pcTRAIMTime;
            unsigned long pcTDTime;
            long month, day, year;
            long hours, minutes, seconds;
            long sawTooth;
            long blatchCount;
            long analogCount;
            long coinCount;
            long alatch;
            long temperature[4];
            long hv[3];
            long blatch[FIFO_SIZE];
            long analog[ANALOG_SIZE];
            long coin[COIN_SIZE];
};

#endif


/*
 *  id
 o record id - bits 0 - 19
 + Incremented once a second by Time Tag interrupt routine
 + A missing values means data has been lost
 + Starts at 0 when program restarted
 o DAC calibration event flag - bit 20
 o LED calibration event flag - bit 21
 o data format - bits 22 - 23, 0 for old format, 1 for new format
 o site id - bits 24 - 29
 + 0 - development system or testing
 + 1 - OLEARY
 + 2 - OBRIEN
 + 3 - TRINITY
 + 4 - MACDONALD
 + 5 - MADDOCK
 + 6 - THORHILD
 + 7 - GPRAIRIE
 + 8 - UOFA
 + 9 - UBC
 o TTAG version - bit 30, 0 for 28 bit TTAG, 1 for 30 bit TTAG
 * deltaTime
 o Milliseconds between interrupts measured by the multimedia timer in windows
 * pcTTTime
 o Milliseconds since computer was booted, for each Time Tag interrupt
 o Starts over at 0 every 49.71 days
 o Should increment by approx 1000, larger increments mean missing 1 sec pulses
 * pcTRAIMTime
 o Milliseconds since computer booted, when sawtooth correction decoded
 o Use this to ensure that sawtooth value stays in sync with the Time Tag data
 * pcTDTime
 o Milliseconds since computer booted, when GPS time & date decoded
 o Use this to ensure that the GPS time & date stays in sync with the sawtooth and Time Tag data
 * month, day, year
 o month, day, & year from GPS
 * hours, minutes, seconds
 o hours, minutes, & seconds from GPS
 * sawTooth
 o sawtooth correction from GPS
 * blatchCount
 o number of values read from Time Tag FIFO
 * analogCount
 o number of values read from Analog module
 * coinCount
 o number of values read from Coincidence module
 * alatch
 o Time Tag count when 1 sec pulse occurred
 * temperature[4]
 o temperature readings for 4 detectors
 o old files are temperature[3]
 * blatch [blatchCount]
 o array of data from Time Tag FIFO  (time of coincidence)
 * analog[analogCount]
 o array of data from Analog module
 * coin[coinCount]
 o array of data from Coincidence module

 The alatch value needs corrected by the sawtooth value as well as the Time Tag oscillator frequency.

 The blatch data needs corrected by the oscillator frequency.

 Due to the jitter in the GPS 1 PPS and the reading of the Time Tag FIFO, the blatch data needs to be
 compared to the alatch data to determine if it belongs in the previous, current, or next 1 second interval.

 Use the following code to determine the count between alatch values taking into account the counter wrapping.

 long delta_a;
 if( pCurrentSecData->alatch > prev->alatch )
 {
 delta_a = pCurrentSecData->alatch - prev->alatch;
 }
 else
 {
 delta_a = m_nMaxCount - prev->alatch + pCurrentSecData->alatch;
}
delta_a *= 10; // convert to nanoseconds

m_nMaxCount = 268435456L for 28 bit time tag.

m_nMaxCount = 1073741824L for 30 bit time tag.

long & unsigned long are 32-bit words

The routine for saving the data to a file:

void CPersistAlta::SaveBlocks( TTAG_DATA* pCurrentSecData )
{
	if( fp == NULL )
	{
		return;
	}
	int longSize = sizeof( long );
	int size = sizeof( TTAG_DATA )
		- longSize * FIFO_SIZE
		- longSize * ANALOG_SIZE
		- longSize * COIN_SIZE;
	fwrite( pCurrentSecData, size, 1, fp );
	fwrite( pCurrentSecData->blatch,
			pCurrentSecData->blatchCount * longSize, 1, fp );
	fwrite( pCurrentSecData->analog,
			pCurrentSecData->analogCount * longSize, 1, fp );
	fwrite( pCurrentSecData->coin,
			pCurrentSecData->coinCount * longSize, 1, fp );
}


*/
#endif

queueElement::queueElement(const queueElement& eq){
	if(eq.data){
		data = new struct TTAG_DATA;
		memcpy( data, eq.data, sizeof(struct TTAG_DATA));
		timeStamp = eq.timeStamp;
		freq = eq.freq;
		alatch = eq.alatch;
		sn = eq.sn;
		wideOutput = eq.wideOutput;
	} else {
		data = NULL;
		timeStamp = 0;
		freq = 0;
		alatch = 0;
		sn = 0;
	}
}

void queueElement::setData( struct TTAG_DATA *t)
{
	if(!t){
		data = NULL;
		timeStamp = 0;
		alatch = 0;
		sn = 0;
		return;
	}
	data = t;
	alatch = data->alatch;
	sn = data->id & 0xfffff;
	if((data->year > 1990) || data->month || data->day){
		struct tm GPSTime;
		memset(&GPSTime,0,sizeof(struct tm));
		GPSTime.tm_sec = data->seconds;
		GPSTime.tm_min = data->minutes;
		GPSTime.tm_hour = data->hours;
		GPSTime.tm_mday = data->day;
		GPSTime.tm_mon = data->month - 1;
		GPSTime.tm_year = data->year - 1900;
		timeStamp = mktime(&GPSTime) + data->sawTooth/1e9;
	} else
		timeStamp = 0;
	return;
}

void queueElement::report()
{
	if(!data){
		cerr << "null event" << endl;
		return;
	}
	cerr.precision(10);
	cerr << "id: " << sn << " ";
	cerr << data->year << " " << data->month << " ";
	cerr << data->day << " " << data->hours << ":";
	cerr << data->minutes << ":" << data->seconds << endl;
	return;
}

void queueElement::process()
{
	if(isDelimiter){
		cout << "x 0 0 0 0 0 0 0.0 0 0 0 0 0 0 0.0 0.0 0.0 0.0";
		if(wideOutput)
			cout << " 0 0.0";
		cout << endl;
		return;
	}
    cerr << "got to queueElement.process";
	if(!data)
		return;
	if(freq < 9e7 || freq > 1.1e8){
		report();
		cerr.precision(10);
		cerr << "Strange frequency = " << freq << endl;
		freq = 1e8;
	}
	if( (data->analogCount != data->coinCount) ||
			(data->analogCount != 3*data->blatchCount) ){
		report();
		cerr << "analog: " << data->analogCount << "; coin: " << data->coinCount;
		cerr << "; blatch: " << data->blatchCount << endl;
		return;
	}
	for( int i = 0;  i < data->blatchCount; i++){
		double blAl = data->blatch[i] - alatch;
		if( blAl > 2e8 )
			blAl -= MAX30;
		if( blAl < -2e8 )
			blAl += MAX30;
		double tc = blAl/freq;
//		double realTimeStamp = timeStamp + tc;
		struct tm eventTime;
		//time_t eT = (time_t)realTimeStamp;
		/*localtime_r( &eT, &eventTime);*/
		if((data->id >> 20 & 3) && ( blAl > 0 ) && ( blAl < 150 ))
			cout << "c ";
		else
			cout << "a ";
		cout << eventTime.tm_year + 1900 << " " << eventTime.tm_mon + 1
		<< " " << eventTime.tm_mday << " " << eventTime.tm_hour << " "
		<< eventTime.tm_min << " " << eventTime.tm_sec << " ";
		cout.precision (10);
	//	fstream a_file ("%userprofile%\\Documents\\COSMIC RAY\\cosmicray\\output\\example.txt");
		//if (a_file.is_open())
		//{
		    cerr << (data->analog[i * 3] & 0x7ff);// << " "
            cerr << (data->analog[i * 3 + 1] & 0x7ff);// << " "
            cerr << (data->analog[i * 3 + 2] & 0x7ff);
		//}

		/*a_file << /*((long long)(round(1e10*(realTimeStamp - eT))+0.5))/10. << " "
		<< (data->coin[i * 3] & 0xfff) << " "
		<< (data->coin[i * 3 + 1] & 0xfff) << " "
		<< (data->coin[i * 3 + 2] & 0xfff) << " "
		<< *///(data->analog[i * 3] & 0x7ff) << " "
		/*<< (data->analog[i * 3 + 1] & 0x7ff) << " "
		<< (data->analog[i * 3 + 2] & 0x7ff)/* << " "
		<< data->temperature[0]/2.0 << " "
		<< data->temperature[1]/2.0 << " "
		<< data->temperature[2]/2.0 << " "
		<< data->temperature[3]/2.0*/;
		//a_file.close();
	}
}

myQueue::myQueue():TicksSize(0),TimeSize(0)
{
}

int myQueue::insert(queueElement *el)
{
	static int badCount = 0;
	int i = 0;
	if(el->getTimeStamp() < 0.1){
		push_front(el);
		badCount++;
		if(badCount > 8){
			badCount = 0;
			return 1;
		}
		return 0;
	} else {
		badCount = 0;
		for( i = 0; i < size(); i++)
			if((*this)[i]->getTimeStamp() > 0.1)
				break;
	}
	if(!size()){
		push_front(el);
		return 0;
	}
	if((*this)[i]->getTimeStamp() < 0.1){
		push_front(el);
		return 0;
	}
	TimeSize += el->getTimeStamp() - (*this)[i]->getTimeStamp();
	int AL = el->getAlatch() - (*this)[i]->getAlatch();
	if( AL < 0)
		AL += MAX30;
	TicksSize += AL;
	push_front(el);
	return 0;
}

void myQueue::eraseLast(int flag)
{
	int i = 0;
	if(!size())
		return;
	if(TicksSize < 0.1){
		if(!flag)
			delete back();
		pop_back();
		return;
	}
	if(back()->getTimeStamp() < 0.1){
		if(!flag)
			delete back();
		pop_back();
		return;
	}
	for( i = size() - 2; i >= 0; i--)
		if((*this)[i]->getTimeStamp() > 0.1)
			break;
	if((*this)[i]->getTimeStamp() < 0.1){
		if(!flag)
			delete back();
		pop_back();
		return;
	}
	TimeSize -= (*this)[i]->getTimeStamp() - back()->getTimeStamp();
	int AL = (*this)[i]->getAlatch() - back()->getAlatch();
	if(AL < 0)
		AL += MAX30;
	TicksSize -= AL;
	if(!flag)
		delete back();
	pop_back();
	return;
}

EQ::EQ():lengthLimit(600)
{
	//putenv("PATH=C:\\Users\\Monish\\Desktop\\COSMIC RAY\\2012_06_26_Ilford");
}

void EQ::insert(queueElement *el)
{
	if(!future.size()){
		future.insert(el);
		return;
	}
	if(((future.front()->id() + 1) & 0xfffff) != el->id()){
		dumpQueue();
		future.insert(el);
		return;
	}
	if(lengthLimit == future.size()){
		future.back()->setFreq( calculateFrequency());
		future.back()->process();
		past.insert(future.back());
		if(past.size() == (lengthLimit + 1))
			past.eraseLast();
		future.eraseLast(1);
	}
	if(future.insert(el))
		dumpQueue();
	return;
}

double EQ::calculateFrequency()
{
	double fF;
	if(future.getTimeSize() > 0.1)
		fF = future.getTicksSize()/future.getTimeSize();
	else
		fF = 0;
	double fP;
	if(past.getTimeSize() > 0.1)
		fP = past.getTicksSize()/past.getTimeSize();
	else
		fP = 0;
	if(( fF < 0.1) || (fP < 0.1))
		return fF + fP;
	else
		return (fF + fP)/2;
}

void EQ::dumpQueue()
{
	while(future.size()){
		future.back()->setFreq(calculateFrequency());
		future.back()->process();
		past.insert(future.back());
		if(past.size() == (lengthLimit + 1))
			past.eraseLast();
		future.eraseLast(1);
	}
	while(past.size())
		past.eraseLast();
	queueElement e;
	e.setDelimiter();
	e.process();
	return;
}

char baseName[64];


int myFilter (const struct dirent *de)
{
	if (!strncmp (baseName, de->d_name, strlen (baseName)))
		return 1;
	else
		return 0;
}

int main( int argc, char **argv)
{
	if (2 != argc)
	{
		cerr << "Usage " << argv[0] << "  BASENAME " << endl;
		cerr << "eg. " << argv[0] << " PRAGUE " << endl;
		cerr << "this means this is the only argument passed via command line." << endl;
		return 1;
	}
	if (!strlen (argv[1]))
	{
		cerr << "Invalid BASENAME" << endl;
		return 1;
	}
	EQ q;
	strncpy( baseName, argv[1], 64);
	struct dirent **dirContent;
	int nFiles = 4;
	//int nFiles = ScanDir(".", &dirContent, myFilter/*, alphasort*/);
	/* Non POSIX, conforms to BSD 5.3 */

	size_t headerSize = sizeof(struct TTAG_DATA)
		- sizeof(uint32_t)*(FIFO_SIZE+ ANALOG_SIZE + COIN_SIZE);
	for (int i = 0; i < nFiles; i++){
		struct stat st;
		if(stat(dirContent[i]->d_name, &st)){
			 free (dirContent[i]);
			 continue;
		}
		if(!S_ISREG(st.st_mode)){
			free (dirContent[i]);
			continue;
		}
		ifstream inp(dirContent[i]->d_name, ios::in);
		if(!inp){
			cerr << "Can not open file: " << dirContent[i]->d_name << endl;
			return 1;
		}
		while(!inp.eof()){
			TTAG_DATA *s = new TTAG_DATA();
			memset( s,0,sizeof(TTAG_DATA));
			int dataError = 0;
			inp.read( (char*)s, headerSize);
			if(inp.eof() && !inp.gcount()){
				delete s;
				break;
			}
			if( (headerSize > inp.gcount()) && inp.gcount() ){
				cerr << "damaged file: " << dirContent[i]->d_name
					<< endl;
				delete s;
				break;
			}
			if( ANALOG_SIZE < s->analogCount){
				dataError = 1;
				cerr << "analog_count = " <<
					s->analogCount << " too high." << endl;
			}
			if( COIN_SIZE < s->coinCount){
				dataError = 1;
				cerr << "coin_count = " <<
					s->coinCount << " too high." << endl;
			}
			if( FIFO_SIZE < s->blatchCount){
				dataError = 1;
				cerr << "blatch_count = " <<
					s->blatchCount << " too high." << endl;
			}
			if(dataError){
				delete s;
				break;
			}
			inp.read((char*)s + headerSize, s->blatchCount*4);
			if( inp.gcount() != s->blatchCount*4){
				cerr << "corrupted blatchCount" << endl;
				delete s;
				break;
			}
			inp.read((char*)s + headerSize+ FIFO_SIZE*4, s->analogCount*4);
			if( inp.gcount() != s->analogCount*4){
				cerr << "corrupted analogCount" << endl;
				delete s;
				break;
			}
			inp.read((char*)s + headerSize + (FIFO_SIZE+ANALOG_SIZE)*4,
						s->coinCount*4);
			if( inp.gcount() != s->coinCount*4 ){
				cerr << "corrupted coinSize " << endl;
				delete s;
				break;
			}
			queueElement *x = new queueElement();
			x->setData( s);
			q.insert(x);
		}
		inp.close();
		free (dirContent[i]);
	}
	free (dirContent);
	cin.get();
	return 0;
}
