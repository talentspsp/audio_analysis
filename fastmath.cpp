#include <stdexcept>
#include <cstdlib>
#include <cmath>
using namespace std;

template <class T>
inline FMmatrix<T>::FMmatrix(int in_row, int in_col, T val)
{
  if(in_row<=0 || in_col<=0)
    throw runtime_error("row and column must be positive!");
  row=in_row;
  col=in_col;
  data=new T[row*col];
  for(int i=0;i<row*col;i++)
    data[i]=val;
}

template <class T>
inline FMmatrix<T>::FMmatrix(const FMmatrix& rhs): row(rhs.row), col(rhs.col)
{
  if(row>0)
    {
      data=new T[row*col];
      for(int i=0;i<row*col;i++)
	data[i]=rhs.data[i];
    }
}
template <class T>
inline FMmatrix<T>& FMmatrix<T>::operator= (const FMmatrix& rhs)
{
  if(row != 0)
    delete []data;
  row=rhs.row;
  col=rhs.col;
  if(row>0)
    {
      data=new T[row*col];
      for(int i=0;i<row*col;i++)
	data[i]=rhs.data[i];
    }
}

template <class T>
inline void FMmatrix<T>::init(int in_row, int in_col)
{
  if(data!=0)
    throw runtime_error("can't use init to already initialized objects!");
  row=in_row;
  col=in_col;
  data=new T[row*col];
}
template <class T>
inline T FMmatrix<T>::getval(int r, int c)
{
  if(r<0 || r>=row || c<0 || c>=col)
    throw runtime_error("index out of range!");
  return data[r*col+c];
}

template <class T>
inline void FMmatrix<T>::setval(int r, int c, T val)
{
   if(r<0 || r>=row || c<0 || c>=col)
     throw runtime_error("index out of range!");
   data[r*col+c]=val;
}


template <class T>
inline void scalar_mult_mat(T sc, const FMmatrix<T>& mat, FMmatrix<T>& result)
{
  if((result.row != mat.row || result.col != mat.col) && result.row !=0 )
    throw runtime_error("result and mat's dimensions don't match!");
  if(result.row==0)
    {
      result.row=mat.row;
      result.col=mat.col;
      result.data=new T[result.row*result.col];
    }
  for(int i=0;i<result.row;i++)
    for(int j=0;j<result.col;j++)
      result.data[i*result.col+j]=mat.data[i*mat.col+j]*sc;
}

template <class T>
inline void mat_add(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B)
{
  //check validation
  if(A.row != B.row || A.col != B.col)
    throw runtime_error("the dimensions of the adding matrices don't match!");
  if((result.row != A.row || result.col != A.col) && result.row !=0)
    throw runtime_error("the dimension of the result doesn't match that of the adding matrices!");
  if(result.row ==0)
    {
      result.row=A.row;
      result.col=A.col;
      result.data=new T[result.row*result.col];
    }
  for(int i=0;i<result.row;i++)
    for(int j=0;j<result.col;j++)
      result.data[i*result.col+j]=A.data[i*result.col+j]+B.data[i*result.col+j];
}

template <class T>
inline void ew_mult(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B)
{
   //check validation
  if(A.row != B.row || A.col != B.col)
    throw runtime_error("the dimensions of the element-wise multiplying matrices don't match!");
  if((result.row != A.row || result.col != A.col) && result.row !=0)
    throw runtime_error("the dimension of the result doesn't match that of the element-wise multiplying matrices!");
  if(result.row ==0)
    {
      result.row=A.row;
      result.col=A.col;
      result.data=new T[result.row*result.col];
    }
  for(int i=0;i<result.row;i++)
    for(int j=0;j<result.col;j++)
      result.data[i*result.col+j]=A.data[i*result.col+j]*B.data[i*result.col+j];
}

template <class T>
inline void ew_div(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B)
{
   //check validation
  if(A.row != B.row || A.col != B.col)
    throw runtime_error("the dimensions of the element-wise dividing matrices don't match!");
  if((result.row != A.row || result.col != A.col) && result.row !=0)
    throw runtime_error("the dimension of the result doesn't match that of the element-wise dividing matrices!");
  if(result.row ==0)
    {
      result.row=A.row;
      result.col=A.col;
      result.data=new T[result.row*result.col];
    }
  for(int i=0;i<result.row;i++)
    for(int j=0;j<result.col;j++)
      result.data[i*result.col+j]=A.data[i*result.col+j]/B.data[i*result.col+j];
}

template <class T>
inline FMmatrix<T> mean(const FMmatrix<T>& A,int dim)
{
  T sum;
  int i,j;
  FMmatrix<T> ret;
  switch(dim)
    {
    case 0:
      ret.init(1,1);
      sum=0;
      for(i=0;i<A.row;i++)
	for(j=0;j<A.col;j++)
	  sum+=A.data[i*A.col+j];
      ret.data[0]=sum/(A.col*A.row);
      break;
    case 1:
      ret.init(1,A.col);
      for(j=0;j<A.col;j++)
	{
	  sum=0;
	  for(i=0;i<A.row;i++)
	    sum+=A.data[i*A.col+j];
	  ret.data[j]=sum/A.row;
	}
      break;
    case 2:
      ret.init(A.row,1);
      for(i=0;i<A.row;i++)
	{
	  sum=0;
	  for(j=0;j<A.col;j++)
	    sum+=A.data[i*A.col+j];
	  ret.data[i]=sum/A.col;
	}
      break;
    default:
      throw runtime_error("no such option for mean!");
    }
  return ret;
}

template <class T>
inline FMmatrix<T> min(const FMmatrix<T>& A, int dim, int* ind)
{
  T m;
  int i,j;
  FMmatrix<T> ret;
  switch(dim)
    {
    case 0:
      ret.init(1,1);
      m=A.data[0];
      if(ind != 0)
	ind[0]=0;
      for(i=1;i<A.row*A.col;i++)
	if(A.data[i]<m)
	  {
	    m=A.data[i];
	    if(ind != 0)
	      ind[0]=i;
	  }
      ret.data[0]=m;
      break;
    case 1:
      ret.init(1,A.col);
      for(j=0;j<A.col;j++)
	{
	  m=A.data[j];
	  if(ind != 0)
	    ind[j]=0;
	  for(i=1;i<A.row;i++)
	    if(A.data[i*A.col+j]<m)
	      {
		m=A.data[i*A.col+j];
		if(ind != 0)
		  ind[j]=i;
	      }
	  ret.data[j]=m;
	}
      break;
    case 2:
      ret.init(A.row,1);
      for(i=0;i<A.row;i++)
	{
	  m=A.data[i*A.col];
	  if(ind != 0)
	    ind[i]=0;
	  for(j=1;j<A.col;j++)
	    if(A.data[i*A.col+j]<m)
	      {
		m=A.data[i*A.col+j];
		if(ind != 0)
		  ind[i]=j;
	      }
	  ret.data[i]=m;
	}
      break;
    default:
      throw runtime_error("no such option for min!");
    }
  return ret;
}

template <class T>
inline FMmatrix<T> max(const FMmatrix<T>& A, int dim, int* ind=0)
{
  T m;
  int i,j;
  FMmatrix<T> ret;
  switch(dim)
    {
    case 0:
      ret.init(1,1);
      m=A.data[0];
      if(ind != 0)
	ind[0]=0;
      for(i=1;i<A.row*A.col;i++)
	if(A.data[i]>m)
	  {
	    m=A.data[i];
	    if(ind != 0)
	      ind[0]=i;
	  }
      ret.data[0]=m;
      break;
    case 1:
      ret.init(1,A.col);
      for(j=0;j<A.col;j++)
	{
	  m=A.data[j];
	  if(ind != 0)
	    ind[j]=0;
	  for(i=1;i<A.row;i++)
	    if(A.data[i*A.col+j]>m)
	      {
		m=A.data[i*A.col+j];
		if(ind != 0)
		  ind[j]=i;
	      }
	  ret.data[j]=m;
	}
      break;
    case 2:
      ret.init(A.row,1);
      for(i=0;i<A.row;i++)
	{
	  m=A.data[i*A.col];
	  if(ind != 0)
	    ind[i]=0;
	  for(j=1;j<A.col;j++)
	    if(A.data[i*A.col+j]>m)
	      {
		m=A.data[i*A.col+j];
		if(ind != 0)
		  ind[i]=j;
	      }
	  ret.data[i]=m;
	}
      break;
    default:
      throw runtime_error("no such option for min!");
    }
  return ret;
}

template <>
inline void FMmatrix<double>::avoidzero()
{
  for(int i=0;i<row*col;i++)
    data[i]=(data[i]<MIN_DOUBLE)?MIN_DOUBLE:data[i];
}

template <class T>
inline FMmatrix<T> norm2(const FMmatrix<T>& A, int dim)
{
  T sum;
  int i,j;
  FMmatrix<T> ret;
  switch(dim)
    {
    case 0:
      ret.init(1,1);
      sum=0;
      for(i=0;i<A.row*A.col;i++)
	sum+=pow(A.data[i],2);
      ret.data[0]=sqrt(sum);
      break;
    case 1:
      ret.init(1,A.col);
      for(j=0;j<A.col;j++)
	{
	  sum=0;
	  for(i=0;i<A.row;i++)	    
	    sum+=pow(A.data[i*A.col+j],2); 
	  ret.data[j]=sqrt(sum);
	}
      break;
    case 2:
      ret.init(A.row,1);
      for(i=0;i<A.row;i++)
	{
	  sum=0;
	  for(j=0;j<A.col;j++)
	    sum+=pow(A.data[i*A.col+j],2);
	  ret.data[i]=sqrt(sum);
	}
      break;
    default:
      throw runtime_error("no such option for min!");
    }
  return ret;
}

template <class T>
void FMmatrix<T>::printmat(const std::ostream& ou)
{
  if(data==0)
    cout<<"the matrix is empty!"<<endl;
  else
    for(int i=0;i<row;i++)
      {
	for(int j=0;j<col;j++)
	  cout<<data[i*col+j]<<'\t';
	cout<<endl;
      }
}


template <class T>
void MatrixMultiply(T *C, T *A, T *B, int hA, int wA, int wB, int crossover)
{
  int m,n,k;
  T sum;
  int hB=wA;
  //T* temp=new T[hA*wB];
  if (hA<crossover || wA<crossover || wB<crossover) {
    
    for (m=0; m<hA; m++) {
      for (n=0; n<wB; n++) {
	sum=0;
	for (k=0; k<wA; k++) {
	  sum+=A[m*wA+k]*B[k*wB+n];
	}
	C[m*wB+n]=sum;
      }
    }
    /*for (m=0; m<hA; m++) {
      for (n=0; n<wB; n++) {
      C[m*wB+n]=temp[m*wB+n];
      }
      }*/
    return;
  }
  int halfhA=(int)(hA/2);
  int halfwA=(int)(wA/2);
  int halfhB=halfwA;
  int halfwB=(int)(wB/2);
  T* R1=new T[halfhA*halfwA];
  T* R2=new T[halfhB*halfwB];
  T* R3=new T[halfhA*halfwB];
  T* R4=new T[halfhA*halfwB];
  
  //To reduce space, the computation below, such as Matrix Add, is implemented in this function
  
  //R1=A21+A22
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwA; n++) {
      R1[m*halfwA+n]=A[(halfhA+m)*wA+n]+A[(halfhA+m)*wA+halfwA+n];
      //R1.data.push_back(A.data[(R1.size+m)*A.size+n]+A.data[(R1.size+m)*A.size+R1.size+n]);
    }
  }
  
  //R2=B12-B11
  for (m=0; m<halfhB; m++) {
    for (n=0; n<halfwB; n++) {
      R2[m*halfwB+n]=B[m*wB+halfwB+n]-B[m*wB+n];
      //R2.data.push_back(B.data[m*B.size+R2.size+n]-B.data[m*B.size+n]);
    }
  }
  
  //R3=R1*R2
  MatrixMultiply(R3,R1,R2,halfhA, halfwA, halfwB);
  //R3=MatrixMultiply2(R1,R2);
  
  //C12=R3, C22=R3
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      C[m*wB+halfwB+n]=R3[m*halfwB+n];
      //C.data[m*C.size+R3.size+n]=R3.data[m*R3.size+n];
      C[(halfhA+m)*wB+halfwB+n]=R3[m*halfwB+n];
      //C.data[(R3.size+m)*C.size+R3.size+n]=R3.data[m*R3.size+n];
    }
  }
  
  //R1=R1-A11
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwA; n++) {
      R1[m*halfwA+n]=R1[m*halfwA+n]-A[m*wA+n];
      //R1.data[m*R1.size+n]=R1.data[m*R1.size+n]-A.data[m*A.size+n];
    }
  }
  
  //R2=B22-R2
  for (m=0; m<halfhB; m++) {
    for (n=0; n<halfwB; n++) {
      R2[m*halfwB+n]=B[(halfhB+m)*wB+halfwB+n]-R2[m*halfwB+n];
      //R2.data[m*R2.size+n]=B.data[(R2.size+m)*B.size+R2.size+n]-R2.data[m*R2.size+n];
    }
  }
  
  //R3=A11*B11, see whether can it be optimized
  T* SubA=new T[halfhA*halfwA];
  T* SubB=new T[halfhB*halfwB];
  
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwA; n++) {
      SubA[m*halfwA+n]=A[m*wA+n];
      
    }
  }
  
  for (m=0; m<halfhB; m++) {
    for (n=0; n<halfwB; n++) {
      SubB[m*halfwB+n]=B[m*wB+n];
    }
  }
  MatrixMultiply(R3,SubA,SubB,halfhA,halfwA,halfwB);
  
  //R3=MatrixMultiply2(A.SubMat(1, 1),B.SubMat(1, 1));
  
  //C11=R3
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      C[m*wB+n]=R3[m*halfwB+n];
      //C.data[m*C.size+n]=R3.data[m*R3.size+n];
    }
  }
  
  //R3=R3+R1*R2, can it be optimized
  //R3=R3+MatrixMultiply2(R1,R2);
  
  MatrixMultiply(R4,R1,R2,halfhA, halfwA, halfwB);
  //R4=MatrixMultiply2(R1,R2);
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      R3[m*halfwB+n]=R3[m*halfwB+n]+R4[m*halfwB+n];
      //R3.data[m*R3.size+n]=R3.data[m*R3.size+n]+R4.data[m*R3.size+n];
    }
  }
  
  //R1=A12-R1
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwA; n++) {
      R1[m*halfwA+n]=A[m*wA+halfwA+n]-R1[m*halfwA+n];
      //R1.data[m*R1.size+n]=A.data[m*A.size+R1.size+n]-R1.data[m*R1.size+n];
    }
  }
  
  //R2=B21-R2
  for (m=0; m<halfhB; m++) {
    for (n=0; n<halfwB; n++) {
      R2[m*halfwB+n]=B[(halfhB+m)*wB+n]-R2[m*halfwB+n];
      //R2.data[m*R2.size+n]=B.data[(R2.size+m)*B.size+n]-R2.data[m*R2.size+n];
    }
  }
  
  //C12=C12+R1*B22
  for (m=0; m<halfhB; m++) {
    for (n=0; n<halfwB; n++) {
      SubB[m*halfwB+n]=B[(halfhB+m)*wB+halfwB+n];
      //R4.data[m*R4.size+n]=B.data[(R4.size+m)*B.size+R4.size+n];
    }
  }
  
  MatrixMultiply(R4,R1,SubB,halfhA, halfwA, halfwB);
  //R4=MatrixMultiply2(R1,R4);
  
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      C[m*wB+halfwB+n]=C[m*wB+halfwB+n]+R4[m*halfwB+n];
      //C.data[m*C.size+R4.size+n]=C.data[m*C.size+R4.size+n]+R4.data[m*R4.size+n];
    }
  }
  
  //C12=C12+R3
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      C[m*wB+halfwB+n]=C[m*wB+halfwB+n]+R3[m*halfwB+n];
      //C.data[m*C.size+R3.size+n]=C.data[m*C.size+R3.size+n]+R3.data[m*R3.size+n];
    }
  }
  
  //C21=A22*R2
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwA; n++) {
      SubA[m*halfwA+n]=A[(halfhA+m)*wA+halfwA+n];
      //R4.data[m*R4.size+n]=A.data[(R4.size+m)*A.size+R4.size+n];
    }
  }
  
  MatrixMultiply(R4,SubA,R2,halfhA, halfwA, halfwB);
  //R4=MatrixMultiply2(R4,R2);
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      C[(halfhA+m)*wB+n]=R4[m*halfwB+n];
      //C.data[(R4.size+m)*C.size+n]=R4.data[m*R4.size+n];
    }
  }
  
  //R1=A11-A21
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwA; n++) {
      R1[m*halfwA+n]=A[m*wA+n]-A[(halfhA+m)*wA+n];
      //R1.data[m*R1.size+n]=A.data[m*A.size+n]-A.data[(R1.size+m)*A.size+n];
    }
  }
  
  //R2=B22-B12
  for (m=0; m<halfhB; m++) {
    for (n=0; n<halfwB; n++) {
      R2[m*halfwB+n]=B[(halfhB+m)*wB+halfwB+n]-B[m*wB+halfwB+n];
      //R2.data[m*R2.size+n]=B.data[(R2.size+m)*B.size+R2.size+n]-B.data[m*B.size+R2.size+n];
    }
  }
  
  //R3=R3+R1*R2
  MatrixMultiply(R4,R1,R2,halfhA, halfwA, halfwB);
  //R4=MatrixMultiply2(R1,R2);
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      R3[m*halfwB+n]=R3[m*halfwB+n]+R4[m*halfwB+n];
      //R3.data[m*R3.size+n]=R3.data[m*R3.size+n]+R4.data[m*R3.size+n];
    }
  }
  
  //C21=C21+R3
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      C[(halfhA+m)*wB+n]=C[(halfhA+m)*wB+n]+R3[m*halfwB+n];
      //C.data[(R3.size+m)*C.size+n]=C.data[(R3.size+m)*C.size+n]+R3.data[m*R3.size+n];
    }
  }
  //C22=C22+R3
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      C[(halfhA+m)*wB+halfwB+n]=C[(halfhA+m)*wB+halfwB+n]+R3[m*halfwB+n];
      //C.data[(R3.size+m)*C.size+R3.size+n]=C.data[(R3.size+m)*C.size+R3.size+n]+R3.data[m*R3.size+n];
    }
  }
  
  //C11=C11+A12*B21
  //R1=A12,R2=B21,R3=R1*R2,C11=C11+R3
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwA; n++) {
      R1[m*halfwA+n]=A[m*wA+halfwA+n];
      //R1.data[m*R1.size+n]=A.data[m*A.size+R1.size+n];
    }
  }
  for (m=0; m<halfhB; m++) {
    for (n=0; n<halfwB; n++) {
      R2[m*halfwB+n]=B[(halfhB+m)*wB+n];
      //R2.data[m*R2.size+n]=B.data[(R2.size+m)*B.size+n];
    }
  }
  
  MatrixMultiply(R3,R1,R2,halfhA, halfwA, halfwB);
  //R3=MatrixMultiply2(R1,R2);
  for (m=0; m<halfhA; m++) {
    for (n=0; n<halfwB; n++) {
      C[m*wB+n]=C[m*wB+n]+R3[m*halfwB+n];
      //C.data[m*C.size+n]=C.data[m*C.size+n]+R3.data[m*R3.size+n];
    }
  }
  
  /*for (m=0; m<size; m++) {
    for (n=0; n<size; n++) {
    C[m*size+n]=C[m*size+n];
    }
    }*/
  
  //dynamic padding
  //C11
  int thA=(hA%2==1)?(hA-1):hA;
  int twB=(wB%2==1)?(wB-1):wB;
  if (wA%2==1) {
    for (m=0; m<thA; m++) {
      for (n=0; n<twB; n++) {
	C[m*wB+n]+=A[m*wA+wA-1]*B[(hB-1)*wB+n];
      }
    }
    
  }
	
  //c12
  //	int thB=(hB%2==1)?(hB-1):hB;
  int twA=(wA%2==1)?(wA-1):wA;
  if (wB%2==1) {
    for (m=0; m<thA; m++) {
      sum=0;
      for (n=0; n<twA; n++) {
	sum+=A[m*wA+n]*B[n*wB+wB-1];
      }
      C[m*wB+wB-1]=sum;
    }
    if (wA%2==1) {
      for (m=0; m<thA; m++) {
	C[m*wB+wB-1]+=A[m*wA+wA-1]*B[(hB-1)*wB+wB-1];
      }
    }
  }
  
  //c21
  if (hA%2==1) {
    for (m=0; m<twB; m++) {
      sum=0;
      for (n=0; n<twA; n++) {
	sum+=A[(hA-1)*wA+n]*B[n*wB+m];
      }
      C[(hA-1)*wB+m]=sum;
    }
    if (wA%2==1) {
      for (m=0; m<twB; m++) {
	C[(hA-1)*wB+m]+=A[(hA-1)*wA+wA-1]*B[(hB-1)*wB+m];
      }
    }
  }
  
  //c22
  if ((hA%2==1) && (wB%2==1)) {
    sum=0;
    for (m=0; m<twA; m++) {
      sum+=A[(hA-1)*wA+m]*B[m*wB+wB-1];
    }
    C[(hA-1)*wB+wB-1]=sum;
    if (wA%2==1) {
      C[(hA-1)*wB+wB-1]+=A[(hA-1)*wA+wA-1]*B[(hB-1)*wB+wB-1];
    }
  }
  delete []R1;
  delete []R2;
  delete []R3;
  delete []R4;
  delete []SubA;
  delete []SubB;
}

template <class T>
void mat_mult(FMmatrix<T>& result, const FMmatrix<T>& A, const FMmatrix<T>& B, int crossover)
{
  if(A.col != B.row)
    throw runtime_error("error when calling mat_mult: the dimensions don't match!");
  if( (result.row != A.row || result.col != B.col) && result.data != 0)
    throw runtime_error("error when calling mat_mult: the dimension of result doesn't match the two matrices");
  if(result.data==0)
    result.data=new T[A.row*B.col];
  MatrixMultiply(result.data, A.data, B.data, A.row, A.col, B.col, crossover);
}
