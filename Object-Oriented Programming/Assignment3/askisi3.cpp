#include <iostream>

#include <string>
#include <sstream>

#include <cstdlib>
#include <ctime>

using namespace std;

int N;


enum Movement{ impressionism, expressionism, naturalism };
enum Condition{ bad, good, excellent };

enum Technique{ oil, aquarelle, tempera };
enum Material{ iron, stone, wood };


class Artifacts{
	
	const int index;
	const string creator;
	const int year;
	
	
public:	
	
	Artifacts(int &i,string &s,int &y) : index(i),creator(s),year(y){
		
		cout <<"Creating an instance of Artifacts... \n"<<endl;		
	}
	
virtual ~Artifacts(){cout <<"Destroying an instance of Artifacts... \n"<<endl;}

virtual void getInfo() const{ cout<<"Creator: "<< creator <<"  Year: "<< year <<endl; }
		void getIndex() const{ cout<<"Index: "<< index <<endl; }
		
virtual bool evaluate(Movement &m,Condition c) = 0; 		//pure virtual		
		
	
};


class Masterpiece :public Artifacts{
			
	
//public:
//	enum Movement{ impressionism, expressionism, naturalism };
//	enum Condition{ bad, good, excellent };
	
private: 
	const Movement movement;
	Condition condition;
	
public:		
	Masterpiece(Movement &m,Condition &c,int &i1,string &s1,int &y1) : movement(m), condition(c), Artifacts(i1,s1,y1)
	{ cout<<"Creating an instance of Masterpiece... \n"<<endl; }
	
	~Masterpiece(){ cout <<"Destroying an instance of Masterpiece... \n"<<endl; }
	
	void setCondition(Condition &C){ condition = C;}
	
	Condition getCondition() const{ return condition;}
	Movement getMovement() const{ return movement;}
	
	
	void getInfo() const{
		
		Artifacts::getInfo();
		
		if(movement == impressionism && condition == bad )				cout <<"Movement: Impressionism  Condition: Bad \n";
		else if(movement == impressionism && condition == good ) 		cout <<"Movement: Impressionism  Condition: Good \n";
		else if(movement == impressionism && condition == excellent )	cout <<"Movement: Impressionism  Condition: Excellent \n";
		
		if(movement == expressionism && condition == bad )				cout <<"Movement: Expressionism  Condition: Bad \n";
		else if(movement == expressionism && condition == good )		cout <<"Movement: Expressionism  Condition: Good \n";
		else if(movement == expressionism && condition == excellent )	cout <<"Movement: Expressionism  Condition: Excellent \n";
		
		if(movement == naturalism && condition== bad )					cout <<"Movement: Naturalism  Condition: Bad \n";
		else if(movement == naturalism && condition == good)			cout <<"Movement: Naturalism  Condition: Good \n";
		else if(movement == naturalism && condition == excellent )		cout <<"Movement: Naturalism  Condition: Excellent \n";
				
	}

//virtual bool evaluate(Movement &m,Condition c) = 0; 		//pure virtual
	
};


class Paintings :public Masterpiece{
	
//public: enum Technique{ oil, aquarelle, tempera };

private:
	const float length;
	const float width;
	const Technique technique;
	
public:
	
	Paintings(float& l,float& w,Technique& t,Movement& m,Condition &c,int& i,string &s,int& y )
		:length(l), width(w), technique(t), Masterpiece(m,c,i,s,y)
	{
		cout<<"Creating an instance of Paintings... \n"<<endl;		
	}
	
	~Paintings(){ cout <<"Destroying an instance of Paintings... \n"<<endl;	}	
	
	void getInfo() const{
		
		cout <<"Painting \n";
		Masterpiece::getInfo();
		
		float surface = width * length;
		
		if(technique == oil)				cout <<"Technique: Oil  Surface: "<<surface<<endl;
		else if(technique == aquarelle)		cout <<"Technique: Aquarelle  Surface: "<<surface<<endl;
		else if(technique == tempera)		cout <<"Technique: Tempera  Surface: "<<surface<<endl;
	}
	
	
	bool evaluate(Movement &m,Condition c=good){
		
		if(Masterpiece::getMovement() == m && Masterpiece::getCondition() >= c) return true;
		
		return false;
	}
	
	
};



class Sculptures : public Masterpiece{
	
//public: enum Material{ iron, stone, wood };

private: 
	const float volume;	
	const Material material;
	
public:

	Sculptures(float &v,Material &m1,Movement &m,Condition &c,int &i,string &s,int &y)
		: volume(v),material(m1),Masterpiece(m,c,i,s,y)
	{
		cout <<"Creating an instance of Sculptures... \n"<<endl;
		}		
	
	~Sculptures(){ cout <<"Destroying an instance of Sculptures... \n"<<endl; }
	
	void getInfo() const{
		
		cout<< "Sculpture\n";
		Masterpiece::getInfo();
		
		if(material == iron) {
			
			cout<<"Volume: "<< volume <<" Material: iron \n";
			return;
		}
		else if(material == stone) {
			
		cout<<"Volume: "<< volume <<" Material: stone \n"; 
		return;
		}
		else{
		
			cout<<"Volume: "<< volume <<" Material: wood \n"; 
			return;
		} 
		
		
	}
	
	bool evaluate(Movement &m,Condition c=excellent){
		
		if( (Masterpiece::getMovement() == m) && (Masterpiece::getCondition() == c) ) return true;
		
		return false;	
	}	
};



void auction(Artifacts *a[],Movement &m,Condition &c){
	
	//cout <<"N is : "<< N;
	
	for(int i=0;i<N ;i++){
		
		a[i]->getIndex();
		a[i]->getInfo();
		
		cout << "\n\n\n 		Time to evaluate: ";
		
		
		if(a[i]->evaluate(m,c) == true) cout <<"Artifact accepted \n\n\n";
		else{
		
		cout <<"Artifact not accepted \n\n\n";
		
		}
	}
	
	
	
}


int main(int argc,char *argv[]){
	
	srand(time(NULL));
	
	Movement mov;
	Condition cond;
	
	if( argc < 4 ){
	
	cout << "Not enough arguments\n";
	return -1;
	
	}	
	else{															
		
	N = atoi(argv[1]);
	//cout << "N is " << N << endl;
	
	
	//Movement
	if(argv[2] == "impressionism" )  mov = impressionism;
	else if(argv[2] == "expressionism") mov = expressionism;
	else Movement mo = naturalism;
	
	
	//cout << "M is " << mo <<endl;
	
	
	//Condition
	if(argv[3] == "bad" )  cond = bad;
	else if(argv[3] == "good") cond = good;
	else  cond = excellent;
		
	//cout << "c is "<< co <<endl;
	}
	
	
	Artifacts **a = new Artifacts*[N]; 		//allocating memory
	
	int j=0;
	
	stringstream s1;
	string cr1;
		
		
	for(int i=0;i<N;i++){				//initialising randomly
		
		
		Movement m1;							//impressionism,expressionism,naturalism
		
		int mov_rand = rand() % 3;
		
		if(mov_rand == 0)	m1 = impressionism;
		else if(mov_rand == 1) m1 = expressionism;
		else m1 = naturalism;
		
		
		
		
		Condition c1;								// bad,good,excellent
		
		int cond_rand = rand() % 3;
		
		if(cond_rand == 0)	c1 = bad;
		else if(cond_rand == 1) c1 = good;
		else c1 = excellent;
		
		
		s1 << "Creator" << i ;									// CreatorI , I is integer
		cr1 = s1.str();
		
		int year = rand() % 2018;								// year = [0,2017] 
		
		

		
		j = rand() % 2;					//j=0(paintings) or j=1(sculpture)
		
		if(j==0){					//paintings
			
			float l = ( (float)rand() ) / ( (float)(RAND_MAX/32.0) );	//length
			float w = ( (float)rand() ) / ( (float)(RAND_MAX/32.0) );	//width
			
			Technique T;										//oil,aquarelle,tempera
			int tech_rand = rand() % 3;
			
			
			if(tech_rand == 0)	T = oil;
			else if(cond_rand == 1) T = aquarelle;
			else T = tempera;
			
			
			
			a[i] = new Paintings(l,w,T,m1,c1,i,cr1,year);	
		}
		if(j==1){		//sculpture
		
			float vol = ( (float)rand() ) / ( (float)(RAND_MAX/32.0) );			//volume
			
			Material mat;										//iron,stone wood
			int mat_rand = rand() % 3;
			
			if(mat_rand == 0)	mat = iron;
			else if(mat_rand == 1) mat = stone;
			else mat = wood;
			
			
			
			a[i] = new Sculptures(vol,mat,m1,c1,i,cr1,year);
			
		}
		
		s1.str( std::string() );			//clearing the string stream
		s1.clear();
		
		
	}		//initialisation of array complete!
	
	
cout << "\n\n		Starting auction	\n\n";	
	
auction(a,mov,cond);		
}











