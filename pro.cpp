#include <bits/stdc++.h>
using namespace std;



int adder(string s1, string s2);

int shift(string s1,string s2);

void right_shift(string &s, int amt);

void equalise_exponents(string &s1, string &s2, int shift);

void twos_complement(string &s);

bool addToExpo(string &s, int shift);

string binaryAdd(string &s1, string &s2);

void left_shift(string &s, int amt);

bool normalise_roundoff(string &s, string &e);
int main(){
	int clk = 0;
    while(true){
        // cout << "arg1 : " ;
        string s1;
        cin >>s1;
        if(s1==""){
        	break;
        }
        // cout << endl;
        // cout << "arg2 : ";
        string s2;
        cin >> s2;
        // cout << endl;

        clk += adder(s1,s2);
		cout << endl;
		cout << endl;

    }
    printf("clk time %d\n", clk);
    // cout << endl;

}

bool checkValid(int sign1 ,string &s1e,string &s1f, int sign2, string &s2e,string &s2f){
	bool x = true;
	int r1=-1; //0 fp , 1 den, 2 Nan, 3 inf 
	if(s1e == "11111111"){
		if(s1f == "0100000000000000000000000"){
			r1=3;
		}else{
			r1 = 2;
		}
	}else if(s1e == "00000000"){
		if(s1f != "0100000000000000000000000"){
			r1=1;
		}
	}else{
		r1=0;
	}
	int r2=-1;
	if(s2e == "11111111"){
		if(s2f == "0100000000000000000000000"){
			r2=3;
		}else{
			r2 = 2;
		}
	}else if(s2e == "00000000"){
		if(s2f != "0100000000000000000000000"){
			r2=1;
		}
	}else{
		r2=0;
	}
	if(r1==2 || r2 == 2){
		cout << "Nan";
		x=false;
	}else if(r1==3 || r2 == 3){
		if(r1==3 && r2==3){
			if(sign1 == -1){
				if(sign2 == -1){
					cout << "neg Inf";
				}else{
					cout << "Nan";
				}
			}else{
				if(sign2 == -1){
					cout << "Nan";
				}else{
					cout << "Inf";
				}
			}
		}else{
			if(r1==3){
				if(sign1 == 0){
					cout << "Inf";
				}else{
					cout << "neg Inf";
				}
			}else if(r2==3){
				if(sign2 == 0){
					cout << "Inf";
				}else{
					cout << "neg Inf";
				}
			}
		}
		x=false;
	}
	// cout << endl;
	return x;

}

int adder(string s1, string s2){
	// form arg1
	int clk =0;

    int sign1 = -1*(s1[0]-'0');
    string s1e = "";
    for(int i=1;i<9;i++){
        s1e+=s1[i];
    }

	int o1 = 0;
    string s1f = "01";
    for(int i=9;i<32;i++){
        s1f+=s1[i];
    }

    // form arg2
    int sign2 = -1*(s2[0]-'0');
    string s2e = "";
    for(int i=1;i<9;i++){
        s2e+=s2[i];
    }

	int o2=0;
    string s2f = "01";
    for(int i=9;i<32;i++){
        s2f+=s2[i];
    }
    if(!checkValid(sign1, s1e, s1f, sign2, s2e, s2f)){
		// cout << "Invaid input" << endl;
		return clk;
	}
	if(s1e == "00000000"){
		s1e[7]='1';
		s1f[1]='0';
	}
	if(s2e == "00000000"){
		s2e[7]='1';
		s2f[1]='0';
	}
	
    // step 1
    // compare exponents and equalise them
    cout << sign1 << " " << s1e << " " << s1f << endl;
    cout << sign2 << " " << s2e << " " << s2f << endl;
    int shift_amt  = shift(s1e,s2e);
    if(shift_amt <0){
    	bool x =addToExpo(s1e,abs(shift_amt));
    	if(!x){
    		cout << "exception" << endl;
    		return clk;
    	}
    }else{
    	bool x = addToExpo(s2e,shift_amt);
    	if(!x){
    		cout << "exception" << endl;
    		return clk;
    	}
    }

    // shift the mantissa with smaller exponent
    cout << "shift amt is : " << shift_amt << endl;
    int r1 = abs(shift_amt);
    if(shift_amt < 0 ){
    	char g = s1f[s1f.length()-1];
    	while(r1>0){
    		g = s1f[s1f.length()-1];
    		right_shift(s1f,1);
    		r1--;
    	}
    	if(g=='1'){
    		string s1 = "0000000000000000000000001";
			string s2 = binaryAdd(s1,s1f);
			for(int i=0;i<s2.size();i++){
				s1f[i]=s2[i];
			}
    	}
    }
    if(shift_amt > 0){
    	char g = s2f[s2f.length()-1];
    	while(r1>0){
    		g = s2f[s2f.length()-1];
    		right_shift(s2f,1);
    		r1--;
    	}
    	if(g=='1'){
    		string s1 = "0000000000000000000000001";
			string s2 = binaryAdd(s1,s2f);
			for(int i=0;i<s2.size();i++){
				s2f[i]=s2[i];
			}
    	}
    }
    clk++;
    cout << "after right shifting" << endl;
    cout << s1f << endl;
    cout << s2f << endl;
    // // taking 2s complement if arg is negative
    cout << "add sign bit to front"<<endl;
    string temp1 = "";
	string temp2 = "";
    temp1+= ('0');
    temp1+= s1f;
    temp2+= ('0');
    temp2+=s2f;
    // cout << "new significands" << endl;
    cout << temp1 << endl;
    cout << temp2 << endl;
    if(sign1 ==-1){
    	twos_complement(temp1);
    }
    if(sign2 == -1){
    	twos_complement(temp2);
    }
 
    // add mantisa the two mantissa
    // check for overflow 
    // check sign bit if negative then take twos compliment


	// cout << "new significands after twos compliment" << endl;
    cout << temp1 << endl;
    cout << temp2 << endl;

    string s3 = binaryAdd(temp1,temp2);
	// cout << "result after signed addition" << endl << s3 << endl;
    int sign3 = 0;
	if(s3[0]=='1'){
		sign3=-1;
    	twos_complement(s3);
    }
    // cout << "after taking twos complement of result if needed" << endl;
    string s5="";
    for(int i=1;i<s3.length();i++){
    	s5 += s3[i];
    }
    clk++;
    cout << sign3 <<" "<< s1e << " "<< s5 << endl;
    string e = "";
    e+=s1e;
    if(!normalise_roundoff(s5,e)){
    	return clk;
    }
    clk+=2;
    // cout << "exp  "<< e << " "<< s5 << endl;
    cout << "after normalise" << endl;
    cout << e << " "<< s5 << endl;
    string ans = "0";
    if(sign3 == -1){
    	ans[0]='1';
    }
    ans += e;
    // cout << -1* sign3 << e;
    int j=2;
    // cout << s5.length() << endl;
    while(j<s5.length()){
    	ans+= s5[j];
    	j++;
    }
    cout << ans;
   	return clk;
}


int shift(string s1,string s2){
	int i1=0;
	int p=1;
	for(int i=s1.length()-1;i>=0;i--){
		i1+= (s1[i]-'0')*p;
		p=p*2;
	}
	int i2=0;
	p=1;
	for(int i=s2.length()-1;i>=0;i--){
		i2+= (s2[i]-'0')*p;
		p=p*2;
	}
	return i1-i2;

}


void right_shift(string &s, int amt){
	int i=s.length()- amt-1;
	while(i>=0){
		s[i+amt]=s[i];
		s[i]='0';
		i--;
	}
}

void left_shift(string &s, int amt){
	int i=amt;
	while(i<s.length()){
		s[i-amt]=s[i];
		i++;
	}
}
void twos_complement(string &s){
	for(int i=0;i<s.length();i++){
		if(s[i]=='0'){
			s[i]='1';
		}else{
			s[i]='0';
		}
	}
	int carry=1;
	for(int i=s.length()-1;i>=0;i--){
		int r= ((s[i]-'0')+carry);
		s[i]=r%2 + '0';
		carry= r/2;
	}
}

string convert(int sh){
	int r=abs(sh);
	string s="00000000";
	int i=7;
	while(r>0){
		s[i] = ('0'+r%2);
		r=r/2;
		i--;
	}
	if(sh<0){
		twos_complement(s);
	}
	return s;
}

bool addToExpo(string &s, int shift){
	// cout << "--------------------" << s << endl;
	int e1=0;
	int p=1;
	for(int i=s.length()-1;i>=0;i--){
		e1+= ((s[i]-'0')*p);
		p = p*2;
	}
	e1 += shift;
	// cout << e1 << endl;
	if(e1>=255){
		cout << "Overflow exception";
		return false;
	}
	if(e1<=0){
		cout << "Underflow exception";
		return false;
	}
	// cout << e1 << endl;
	string s1 = convert(e1);
	// cout << s1 << endl;
	int i=0;
	while(i<s1.length()){
		s[i]=s1[i];
		i++;
	}
	return true;
}



string binaryAdd(string &s1, string &s2){
	int i=s1.length()-1;
	int carry =0;
	string s3="";
	while(i>=0){
		int r = (s1[i]+s2[i])- (2*'0');
		r+=carry;
		s3+= (r%2 + '0');
		carry = r/2;
		i--;
	}
	i=0;
	int j=s3.length()-1;
	while(i<j){
		char temp = s3[i];
		s3[i]=s3[j];
		s3[j]=temp;
		i++;
		j--;
	}
	return s3;
}



bool normalise_roundoff(string &s, string &e){
	int i=0;
	while(s[i]!='1' && i<s.length()){
		i++;
	}
	int n=1-i;
	// cout << "offset is "<< n <<endl;
	if(n<0){
		n=abs(n);
		while(n>0){
			if(!addToExpo(e,-1)){
				return false;
			}
			left_shift(s,1);
			n--;
		}
		return true;
	}
	if(n>0){
		// cout << "right" << endl;
		char c=s[24] ;
		while(n>0){
			if(!addToExpo(e,1)){
				return false;
			}
			// cout << e << endl;
			c=s[24];
			right_shift(s,1);
			n--;
		}
		if(c=='1'){
			string s1 = "0000000000000000000000001";
			string s2 = binaryAdd(s1,s);
			for(int i=0;i<s2.length();i++){
				s[i]=s2[i];
			}
			return normalise_roundoff(s,e);
		}
	}
	return true;
}