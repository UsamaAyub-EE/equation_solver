#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
void make_triangular(int choice);
int GCD(int,int);
struct indices
{
	int row,col;
};
#define MAX_SIZE 10	//to ensure that the code is parameterizable. see line 188
vector<string> eq_number{"first","second","third","fourth","fifth","sixth","seventh","eighth",
"nineth","tenth"}; //these equation numbers will help in taking input
//from the user
bool print_steps;
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
		bool operator == (fraction const &);

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
bool fraction::operator ==(fraction const &obj)
{
	return num==obj.num && den==obj.den;
}
indices find_first_one(int i,int n,int m,vector<vector<fraction> > A,int&,vector<int>&);
void make_one(int i,int j,int n,vector<vector<fraction> >& A);
void make_zero(int choice,int i,int j,int m,int n,vector<vector<fraction> >& A);
void row_swap(int i1,int i2,vector<vector<fraction> >& A);
void print_aug_matrix(int m,int n,vector<vector<fraction> > A);
bool is_zero(int m,vector<fraction> B);
bool is_I(int m,int n,vector<vector<fraction> > A);
bool is_consistent(int m,int n,vector<vector<fraction> > A);
bool is_member(int n,int size,vector<int>);
int index(int j,int size,vector<int>);
void find_inverse();
int main()
{
	cout<<"This program solves m number of equations in n variables. It has been made entirely\n"
	"by Usama Ayub (2018-EE-124) and Muhammad Rizwan (2018-EE-38).\n";
	cout<<"If you wish to enter a fraction,for example, simply type 3/4 and press enter to \n"
	"input 3/4\n";
/*this code can be modified to solve as many equations as is needed. parameters and vars will have
 to be modified only. and at line 11, the MAX_SIZE will have to be modified*/
	char vars[]="xyzuvwtslm";	//these are the variable names that will be used when
//printing equations
	vector<string> parameters{"par1","par2","par3","par4","par5","par6","par7","par8",
	"par9","par10"};	//these are the parameters we'll use to print the solutions
//in case of infinitely many solutions
	vector<int> lead_cols(MAX_SIZE);	//to contain column numbers of leading 1s 
	fraction zero(0,1);	//it's purpose is simply to check whether a fraction is 
//greater than zero or not
	fraction minus(-1,1);	//to multiply a fraction by -1
	int m,n,i,j;
	int exit=1,choice;
	while(exit!=0)
	{
		lead_cols.clear();
		cout<<"What do you want to do? You can:\n1-Enter equations to solve\n2-Find determinant\n3-Find inverse"
		" of a matrix\nEnter the corresponding number\n";
		cin>>choice;
		if(choice==2)
		{
			make_triangular(choice);
			cout<<"Enter 0 to exit. Else enter any number to continue\n";
			cin>>exit;
			continue;
		}
		else if(choice==3)
		{
			find_inverse();
			cout<<"\nEnter 0 to exit. Else enter any number to continue\n";
			cin>>exit;
			continue;
		}
		short steps;
		puts("Do you wish to see the steps of Gauss-Jordan elimination.Enter 1 if yes.Else enter any number");
		cin>>steps;
		print_steps=(steps==1);
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
		int curr_col=0;
		for(i=0;i<m;++i)
		{
			indices obj=find_first_one(i,n,m,A,curr_col,lead_cols);
			if(obj.col==-1)
				break;
			make_one(obj.row,obj.col,n,A);
			if(print_steps)
				print_aug_matrix(m,n,A);
			make_zero(choice,obj.row,obj.col,m,n,A);
			if(obj.row==i)
				continue;
			row_swap(i,obj.row,A);
			if(print_steps)
				print_aug_matrix(m,n,A);
		}
		auto no_of_par=n-1-lead_cols.size();	//no_of_par is the number of free variables	
		vector<int> free_col;
		for(auto i=0;i<n-1;++i)
		{
			if(!is_member(i,lead_cols.size(),lead_cols))
			{
				free_col.push_back(i);
			}
		}
		auto B_zero=is_zero(m,B);	//B_zero is true if B is zero
		auto I=is_I(m,n,A);	//I is true if A is identity matrix
		if(B_zero&&I)
		{
			cout<<"The homogeneous system has only the trivial solution as A is non_singular\n";
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
		else if(m>(n-1)&&no_of_par==0)
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
					if(A[p][q].get_num()==1&&A[p][q].get_den()==1&&is_member(q,lead_cols.size(),lead_cols))
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
					if(!is_member(q,lead_cols.size(),lead_cols)&&A[p][q].get_num()!=0)
/*this checks to see if A[p][q] is a free variable*/
					{
						print=true;
						new_line=true;
/*print is true as something is printed on right side of = sign in this block*/
						par_no=index(q,free_col.size(),free_col);
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
				if(!is_member(var_no,lead_cols.size(),lead_cols))
				{
					par_no=index(var_no,free_col.size(),free_col);
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
indices find_first_one(int i,int n,int m,vector<vector<fraction> > A,int& curr_col,vector<int>& lead_cols)
{
	for(;curr_col<(n-1);++curr_col)
	{
		for(auto a=i;a<m;++a)
		{
			if(A[a][curr_col].get_num()!=0)	//to check whether A[i][j] is zero or not
			{
				indices obj;
				obj.row=a;
				obj.col=curr_col;
				lead_cols.push_back(curr_col);
				++curr_col;
				return obj;
			}
		}
	}
	indices null;
	null.col=null.row=-1;
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
	if(print_steps)
		cout<<"Dividing "<<eq_number[i]<<" row by "<<pivot<<"\n";
	for(j=0;j<n;++j)
	{
		A[i][j]=A[i][j]/pivot;
	}
}
void make_zero(int choice,int i,int j,int m,int n,vector<vector<fraction> >& A)
//this function adds a multiple of ith row to each row to make the entries below and above
//A[i][j] zero. So A[i][j] is the only nonzero entry in the jth column
{
	fraction minus(-1,1);
	fraction one(1,1);
	fraction pivot;
	for(auto p=0;p<m;++p)
	{
		if(choice!=1&&p<i)
			continue;
		if(p==i)
			continue;
		else
		{
			if(choice!=1)
			{
				pivot=A[p][j]/A[i][j];
			}
			else
				pivot=A[p][j];
			if(pivot.get_num()==0)
				continue;
			if(print_steps)
			{
				cout<<"Multliplying "<<eq_number[i]<<" row by "<<(minus*pivot)<<" and adding it to ";
				cout<<eq_number[p]<<" row\n";
			}
			for(auto col=0;col<n;++col)
			{
				A[p][col]=minus*pivot*A[i][col]+A[p][col];
			}
			if(print_steps)
				print_aug_matrix(m,n,A);
		}
	}
}
void row_swap(int i1,int i2,vector<vector<fraction> >& A)
{
/*this function swaps rows. it's purpose is to bring the rows with leading 1s to the top 
of the matrix and bring the 0s to the bottom*/
	A[i1].swap(A[i2]);
	if(print_steps)
		cout<<"Interchanging "<<eq_number[i1]<<" and "<<eq_number[i2]<<" rows"<<"\n";
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
	cout<<"\n----------------------------\n";
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
		return false;
	fraction zero(0,1);
	fraction unity(1,1);
	for(auto i=0;i<m;++i)
	{
		for(auto j=0;j<(n-1);++j)
		{
			if(i==j)
			{
				if(A[i][j]==unity)
					continue;
				else
					return false;
			}
			if(i!=j)
			{
				if(A[i][j]==zero)
					continue;
				else
					return false;
			}
		}
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
bool is_member(int n,int size,vector<int> a)
/*this function checks whether n is in a or not*/
{
	for(auto i=0;i<size;++i)
	{
		if(a[i]==n)
			return true;
	}
	return false;
}
int index(int j,int size,vector<int> a)
/*this function returns the index at which j is present in a*/
{
	for(auto i=0;i<size;++i)
	{
		if(a[i]==j)
			return i;
	}
}
/*------------------------------------------*/
void make_triangular(int choice)
{
	print_steps=false;
	auto size=0;
	fraction sign(1,1);
	fraction minus(-1,1);
	cout<<"Enter the size of the matrix\n";
	cin>>size;
	vector<vector<fraction> > A( size , vector<fraction> (size));
	for(auto i=0;i<size;++i)
	{
		for(auto j=0;j<size;++j)
		{
			cout<<"Enter the number in "<<eq_number[i]<<" row "<<eq_number[j]<<" column\n";
			cin>>A[i][j];
		}
	}
	cout<<"The matrix you entered is as:\n";
	for(auto i=0;i<size;++i)
	{
		for(auto j=0;j<size;++j)
		{
			cout<<" "<<A[i][j];
		}
		cout<<"\n";
	}
	int curr_col=0;
	vector<int> lead_cols;
	for(auto i=0;i<size;++i)
	{
		indices obj=find_first_one(i,size+1,size,A,curr_col,lead_cols);
		if(obj.col==-1)
			break;
		make_zero(choice,obj.row,obj.col,size,size,A);
		if(obj.row==i)
			continue;
		else
		{
			sign=sign*minus;
			row_swap(i,obj.row,A);
		}
	}
	if(choice==2)
	{
		fraction det(1,1);
		for(auto i=0;i<size;++i)
		{
			det=det*A[i][i];
		}
		cout<<"The determinant is "<<sign*det<<"\n";
	}
}
/////////////////////find inverse-------------------------------------------------------------------------------------------------------------
void find_inverse()
{
	print_steps=false;
	int size;
	puts("Enter the size of the matrix whose inverse you wish to find");
	cin>>size;
	vector<vector<fraction>> A( size , vector<fraction> (2*size));	//the matrix of the coefficients of the variables
	for(auto i=0;i<size;++i)
	{
		for(auto j=0;j<size;++j)
		{
			cout<<"Enter the number in "<<eq_number[i]<<" row "<<eq_number[j]<<" column\n";
			cin>>A[i][j];
		}
	}
	cout<<"The matrix you entered is as:\n";
	for(auto i=0;i<size;++i)
	{
		for(auto j=0;j<size;++j)
		{
			cout<<" "<<A[i][j];
		}
		cout<<"\n";
	}
	for(auto i=0;i<size;++i)//populating identity matrix
	{
		for(auto j=size;j<2*size;++j)
		{
			A[i][j]=fraction((i==j-size),1);
		}
	}
	int curr_col=0;
	vector<int> lead_cols;
	for(auto i=0;i<size;++i)//Gauss-Jordan elimination 
	{
		indices obj=find_first_one(i,size+1,size,A,curr_col,lead_cols);
		if(obj.col==-1)
			break;
		make_one(obj.row,obj.col,2*size,A);
		make_zero(1,obj.row,obj.col,size,2*size,A);
		if(obj.row==i)
			continue;
		row_swap(i,obj.row,A);
	}
	bool zero_row=false;
	for(auto i=size-1;i>=0;--i)
	{
		if(zero_row)
		{
			puts("This matrix is singular.Its inverse does not exist.");
			return;
		}
		for(auto j=0;j<size;++j)
		{
			if(A[i][j].get_num()!=0)
				break;
			if(A[i][j].get_num()==0&&j==(size-1))
			{
				zero_row=true;
				break;
			}
		}
	}
	puts("The inverse of the matrix you entered is as under:");
	for(auto i=0;i<size;++i)//populating identity matrix
	{
		for(auto j=size;j<2*size;++j)
		{
			cout<<" "<<A[i][j];
		}
		cout<<"\n";
	}
}
