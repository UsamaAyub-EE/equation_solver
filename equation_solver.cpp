#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
int GCD(int,int);
struct indices
{
	int row,col;
};
#define MAX_SIZE 10	//to ensure that the code is parameterizable. see line 180
int a,b;	//only find_first_one() uses these variables
int lead_rows[MAX_SIZE],lead_cols[MAX_SIZE];	//to contain row numbers and column numbers
//of leading 1s 
int row_i,col_i;	//these act as both indices and sizes for lead_rows and lead_cols
class fraction
{
	int num,den;//numerator,denominator
	public:
		fraction();		//default constructor 
		fraction(int,int);		//parameterized constructor
		fraction(const fraction&);		//copy constructor
		~fraction();		//destructor
		int get_den();		//getters and setters
		int get_num();
		void set_num(int);
		void simplify(fraction&);
		friend istream &operator>>( istream  &Input, fraction &obj )
		{
			string input;
			Input>>input;
			int pos=input.find('/');
			if (pos != string::npos)
			{
				obj.num=stoi(input.substr(0,pos));
				obj.den=stoi(input.substr(pos+1,input.length()));
			}
			else
			{
				obj.num=stoi(input);
				obj.den=1;
			}
			return Input;
		}
		friend ostream &operator<<( ostream &output, const fraction &obj)
		{
			if(obj.den==1)
				output<<obj.num;
			else
				output<<obj.num<<"/"<<obj.den;
			return output;
		} 
		fraction operator + (fraction const &);		//overloaded operators
		fraction operator * (fraction const &);
		fraction operator / (fraction const &);
		bool operator > (fraction const &);

};
//the definitions of all the functions or methods declared in the class
fraction::fraction()
{
	num=0;
	den=1;
}
fraction::fraction(int n,int d)
{
	while(d==0)
	{
		puts("The denominator cannot be 0.Please enter a non-zero value for the denominator");
		cin>>d;
	}
	auto gcd=GCD(n,d);	//for simplifying a fraction, you need only divide the
	num=n/gcd;		//numerator and denominator by their GCD
	den=d/gcd;
	if(den<0)	//you know that 3/-4 is ugly. well this part makes it -3/4
	{
		num*=-1;
		den*=-1;
	}
}
fraction::fraction(const fraction& obj)	//copy constructor
{
	auto gcd=GCD(obj.num,obj.den);	//for simplifying a fraction, you need only divide the
	num=obj.num/gcd;		//numerator and denominator by their GCD
	den=obj.den/gcd;
	if(den<0)	//you know that 3/-4 is ugly. well this part makes it -3/4
	{
		num*=-1;
		den*=-1;
	}
}
fraction::~fraction()
{
	num=0;
	den=1;
}
int fraction::get_den()
{
	return den;
}
int fraction::get_num()
{
	return num;
}
void fraction::set_num(int n)
{
	num=n;
}
void fraction::simplify(fraction& obj)
{
	auto gcd=GCD(obj.num,obj.den);//for simplifying a fraction, you need only divide the
	obj.num/=gcd;	//numerator and denominator by their GCD
	obj.den/=gcd;
	if(obj.den<0)
	{
		obj.den*=-1;
		obj.num*=-1;
	}
}
fraction fraction::operator +(fraction const &obj)
{ 
    fraction ret; 
    ret.num = num*obj.den + den*obj.num;//simple formula for adding fractions
    ret.den = den*obj.den;	//a/b + c/d = (a*d + b*c)/b*d
	simplify(ret); 
    return ret; 
}
fraction fraction::operator *(fraction const &obj)
{ 
    fraction ret; 
    ret.num = num*obj.num; //simple formula for multiplying fractions
    ret.den = den*obj.den;	//a/b * c/d = a*c/b*d
	simplify(ret); 
    return ret; 
}
fraction fraction::operator /(fraction const &obj)
{ 
    fraction ret; 
    ret.num = num*obj.den; 
    ret.den = den*obj.num;
	simplify(ret); 
    return ret; 
}
bool fraction::operator >(fraction const &obj)
{
	float f1,f2;
	if(den<0)
	{
		den*=-1;
		num*=-1;
	}
	f1=float(num/den);
	f2=float(obj.num/obj.den);
	if(obj.num==0)
	{
		if(num>0)
			return true;
		else
			return false;
	}
	if(f1>f2)
		return true;
	else
		return false;
}
indices find_first_one(int m,int n,vector<vector<fraction> > A);
void make_one(int i,int j,int n,vector<vector<fraction> >& A);
void make_zero(int i,int j,int m,int n,vector<vector<fraction> >& A);
void row_swap(int i1,int i2,vector<vector<fraction> >& A);
void print_aug_matrix(int m,int n,vector<vector<fraction> > A);
bool is_zero(int m,vector<fraction> B);
bool is_I(int m,int n,vector<vector<fraction> > A);
bool is_consistent(int m,int n,vector<vector<fraction> > A);
bool is_member(int n,int size,int[]);
int index(int j,int size,int[]);
int main()
{
	cout<<"This program solves m number of equations in n variables. It has been made entirely\n"
	"by Usama Ayub and Muhammad Rizwan.\n";
	cout<<"If you wish to enter a fraction,for example, simply type 3/4 and press enter to \n"
	"input 3/4\n";
/*this code can be modified to solve as many equations as is needed. parameters, vars and
eq_number will have to be modified only. and at line 10, the MAX_SIZE will have to be 
modified*/
	vector<string> eq_number{"first","second","third","fourth","fifth","sixth","seventh",
	"eighth","nineth","tenth"}; //these equation numbers will help in taking input
//from the user	
	char vars[]="xyzuvwtslm";	//these are the variable names that will be used when
//printing equations
	vector<string> parameters{"par1","par2","par3","par4","par5","par6","par7","par8",
	"par9","par10"};	//these are the parameters we'll use to print the solutions
//in case of infinitely many solutions
	fraction zero(0,1);	//it's purpose is simply to check whether a fraction is 
//greater than zero or not
	fraction minus(-1,1);	//to multiply a fraction by -1
	int free_col[MAX_SIZE];	//to contain the column numbers of columns 
//with leading ones and free variables
	int m,n,i,j;
	int exit=1;
	while(exit!=0)
	{
		col_i=row_i=a=b=0;
		puts("Enter the number of equations");
		cin>>m;
		puts("Enter the number of variables");
		cin>>n;
		vector<vector<fraction> > A( m , vector<fraction> (n+1));	//the matrix of the 
//coefficients of the variables
		vector<fraction> B(m);	//the matrix to contain the constant terms
		for(i=0;i<m;++i)
		{
			for(j=0;j<n;++j)
			{
				cout<<"Enter the coefficient of "<<vars[j]<<" in "<<eq_number[i]<<" equation\n";
				cin>>A[i][j];
				A[i][j].simplify(A[i][j]);
			}
			cout<<"Enter the constant term for "<<eq_number[i]<<" equation\n";
			cin>>B[i];
			B[i].simplify(B[i]);
		}
		for(i=0;i<m;++i)
		{
			for(j=0;j<n;++j)
			{
				string sign;
				sign=((A[i][j]>zero)?" +":" ");	//you know that for a positive number,
//its + sign is not printed with it. Well this will print it.
				cout<<((j==0)?" ":sign)<<A[i][j]<<"."<<vars[j];
			}
			cout<<" = "<<B[i]<<"\n";
		}
		for(i=0;i<m;++i)	//this is for making the augmented matrix in which the matrix
//B is added to the right of matrix A
		{
			A[i][n]=B[i];
		}
		++n;	//this particular line is very important. Now, the value of n is the number of
//columns in augmented matrix 
		for(i=0;i<m;++i)
		{
			indices obj=find_first_one(m,n,A);
			if(obj.col==-1)
				break;
			make_one(obj.row,obj.col,n,A);
			print_aug_matrix(m,n,A);
			cout<<"\n----------------------------\n";
			make_zero(obj.row,obj.col,m,n,A);
			print_aug_matrix(m,n,A);
			cout<<"\n----------------------------\n";
			if(obj.row==i)
				continue;
			row_swap(i,obj.row,A);
			print_aug_matrix(m,n,A);
			cout<<"\n----------------------------\n";
		}
		auto no_of_par=n-1-col_i;	//no_of_par is the number of free variables	
		for(auto i=0,ind=0;i<n-1;++i)
		{
			if(!is_member(i,col_i,lead_cols))
			{
				free_col[ind++]=i;
			}
		}
		auto B_zero=is_zero(m,B);	//B_zero is true if B is zero
		auto I=is_I(m,n,A);	//I is true if A is identity matrix
		if(B_zero&&I)
		{
			cout<<"The homogeneous system has no solution as A is non_singular\n";
		}
		else if(!is_consistent(m,n,A))
		{
			cout<<"The system is inconsistent\n";
		}
		else if(I&&!B_zero)
		{
			cout<<"The solution is as under\n";
			for(auto p=0;p<m;++p)
			{
				cout<<vars[p]<<" = "<<A[p][n-1]<<"\n";
			}
		}
		else if(m>(n-1)&&!B_zero&&no_of_par==0)
		{
			cout<<"The solution is as under\n";
			for(auto p=0;p<(n-1);++p)
			{
				cout<<vars[p]<<" = "<<A[p][n-1]<<"\n";
			}
		}
		else if(no_of_par!=0)
		{
			cout<<"The solution is as under\n";
			auto p=0,q=0,par_no=0;
			auto pass=false,print=false,new_line=false;
			for(p=0;p<m;++p)
			{
				pass=false;
				print=false;
				for(q=0;q<(n-1);++q)
				{
//this if checks to see if A[p][q] is a leading 1
					if(A[p][q].get_num()==1&&A[p][q].get_den()==1&&is_member(q,(n-1-no_of_par),lead_cols))
					{
						cout<<vars[q]<<" = ";
						pass=true;
/*pass is true if a leading 1 is found and a variable for the leading 1 is print above*/
						print=false;
/*print is false as nothing has yet been printed on the right side of the = sign*/
					}
					if(!pass)
/*if a leading 1 is not found yet, then continue*/
						continue;
					if(!is_member(q,(n-1-no_of_par),lead_cols)&&A[p][q].get_num()!=0)
/*this checks to see if A[p][q] is a free variable*/
					{
						print=true;
						new_line=true;
/*print is true as something is printed on right side of = sign in this block*/
						par_no=index(q,(no_of_par+1),free_col);
						string sign;
						sign=(!(A[p][q]>zero)?" +":" ");
						cout<<sign<<(minus*A[p][q])<<"."<<parameters[par_no];
					}
				}
				if(pass&&A[p][q].get_num()!=0)
				{
					string sign;
					sign=((A[p][q]>zero)?" +":" ");
					cout<<sign<<A[p][q]<<"\n";
					new_line=false;
				}
				else if(pass&&!print)
				{
					cout<<"0\n";
					new_line=false;
				}
				else if(pass&&new_line)
					cout<<"\n";
				else;
			}
/*this section prints all the parameters used in the solution*/
			for(auto var_no=0;var_no<(n-1);++var_no)
			{
				if(!is_member(var_no,(n-1-no_of_par),lead_cols))
				{
					par_no=index(var_no,(no_of_par+1),free_col);
					cout<<vars[var_no]<<" = "<<parameters[par_no]<<"\n";
				}
			}
			cout<<"Where par stands for parameters which belong to the set of real numbers\n";
		}
		else;
		cout<<"Enter 0 to exit. Else enter any number to continue\n";
		cin>>exit;
	}
	return 0;
}
int GCD(int n,int m)
{	//recursive implementation of Euclid's algorithm to find GCD
	if(m!=0)
	{
		return GCD(m,n%m);
	}
	else
	{
		return n;
	}
}
//this function returns the indices of the first non-zero number in a column
//if there is no non zero number in first column, it moves to the next column to the 
//point it finds a nonzero number and then returns the indices of that number 
indices find_first_one(int m,int n,vector<vector<fraction> > A)
{
	for(;b<(n-1);++b)
	{
		for(a=0;a<m;++a)
		{
			if(A[a][b].get_num()!=0&&!is_member(a,row_i,lead_rows))	//to check whether A[i][j] is zero or not
			{
				indices obj;
				obj.row=a;
				obj.col=b;
				lead_rows[row_i++]=a;
				lead_cols[col_i++]=b;
				++b;
				return obj;
			}
		}
	}
	indices null;
	null.col=null.row=-1;
	a=b=0;
	return null;	//null will be returned in case of a null matrix or when the end of a
//matrix is reached
}
void make_one(int i,int j,int n,vector<vector<fraction> >& A)
//In gaussian elimination, it's better to make the leading entries 1 and this 
//function does that.
{
	fraction pivot=A[i][j];
	if(pivot.get_num()==1&&pivot.get_den()==1)
		return;
	for(j=0;j<n;++j)
	{
		A[i][j]=A[i][j]/pivot;
	}
}
void make_zero(int i,int j,int m,int n,vector<vector<fraction> >& A)
//this function adds a multiple of ith row to each row to make the entries below and above
//A[i][j] zero. So A[i][j] is the only nonzero entry in the jth column
{
	fraction minus(-1,1);
	for(auto p=0;p<m;++p)
	{
		if(p==i)
			continue;
		else
		{
			fraction pivot=A[p][j];
			if(pivot.get_num()==0)
				continue;
			for(auto col=0;col<n;++col)
			{
				A[p][col]=minus*pivot*A[i][col]+A[p][col];
			}
		}
	}
}
void row_swap(int i1,int i2,vector<vector<fraction> >& A)
{
/*this function swaps rows. it's purpose is to bring the rows with leading 1s to the top 
of the matrix and bring the 0s to the bottom*/
	A[i1].swap(A[i2]);
	if(is_member(i1,row_i,lead_rows))
	{
		lead_rows[index(i1,row_i,lead_rows)]=i2;
	}
	if(is_member(i2,row_i,lead_rows))
	{
		lead_rows[index(i2,row_i,lead_rows)]=i1;
	}
}
void print_aug_matrix(int m,int n,vector<vector<fraction> > A)
{
	for(auto p=0;p<m;++p)
	{
		for(auto q=0;q<n;++q)
		{
			if(q==(n-1))
			{
				cout<<" :";
			}
			cout<<" "<<A[p][q]<<" ";
		}
		cout<<"\n";
	}
}
bool is_zero(int m,vector<fraction> B)
/*this function checks to see if B is zero or not*/
{
	for(auto i=0;i<m;++i)
	{
		if(B[i].get_num()!=0)
			return false;
	}
	return true;
}
bool is_I(int m,int n,vector<vector<fraction> > A)
/*this function checks if A is identity matrix or not*/
{
	if(m!=(n-1))
		return 0;
	int sum=0,j=0;
	for(auto i=0;i<m;++i)
	{
		sum=0;
		for(j=0;j<(n-1);++j)
		{
			sum+=A[i][j].get_num();
		}
		if(sum==1)
			continue;
		else
			return false;
	}
	return true;
}
bool is_consistent(int m,int n,vector<vector<fraction> > A)
/*this function checks to see if any row of zeros in A is equal to something other than 0
in the right most column in augmented matrix*/
{
	int count=0,j=0,i=0;
	for(i=0;i<m;++i)
	{
		count=0;
		for(j=0;j<(n-1);++j)
		{
			if(A[i][j].get_num()==0)
			++count;
			else
				break;
		}
		if(count==(n-1)&&A[i][n-1].get_num()!=0)
			return false;
	}
	return true;
}
bool is_member(int n,int size,int a[])
/*this function checks whether n is in a[] or not*/
{
	for(auto i=0;i<size;++i)
	{
		if(a[i]==n)
			return true;
	}
	return false;
}
int index(int j,int size,int a[])
/*this function returns the index at which j is present in a[]*/
{
	for(auto i=0;i<size;++i)
	{
		if(a[i]==j)
			return i;
	}
}
