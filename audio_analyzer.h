#ifndef _AUDIO_ANALYZER_H
#define _AUDIO_ANALYZER_H
#include "spuc/generic/complex.h"
#include <vector>

using namespace SPUC;
using namespace std;

struct datablk //data block
{
  size_t row;
  size_t col;
  double* data;
};

//Notice: this class won't free the memory it used because it may copy some pointer outside this class, so please free the memory used manually rather than using destructor
class audio_analyzer
{
public:
  audio_analyzer(size_t nclass=1);
  ~audio_analyzer() {}
  //label is the class label, if label is -1, it is used for test. return class label
  int readindata(char* filename, int label=-1); // if fail, return negative value

  // plca on the STFT_double, which is from data just read in
  void plca_on_data();

  //STFT on the data just read in
  void do_stft();

  //change the result of STFT from complex number to double
  void stftcpx2double(int option=0);

  //plca on the components of each class, depth is the number of plca applied, if depth = 0, just copy the data in class_comps to extracted_comps, allocate memory for each
  //whole class, currently only support depth 0 and 1, will implement more in future
  bool hierarchical_plca(size_t depth, size_t ncomps, double thd_gpz, double sz);
  
  //add components from plca, P(f|z), to class_comps based on curr_label, if the p(z) corresponding to the component is greater that thd_pz, return number of components added
  size_t add_comps();
  //get the Pt(s) and Pt(z|s) of each source and components, if opt_get_cw==1, use STFT data, if opt_get_cw==2, use all P(f|z) from plca, if opt_get_cw==3, use P(f|z) with p(z) greater than threshold
  bool get_comp_weight();
  //predict the class of the input data. compare the weight of different sources, and classify the data with the class with the highest weight.set label_pred, and also return it
  int pred_max_weight();

  //this group of functions used to get and set parameters
  complex<double>* get_data() {return data;}
  void set_data(complex<double>* in_data) {data=in_data;}
  
  double get_fz() {return fz;}
  void set_fft_param(int in_fftN, int in_fftW, int in_fftH) {fftN=in_fftN; fftW=in_fftW; fftH=in_fftH;}
  int get_lenf() {return lenf;}
  int get_lent() {return lent;}
  int get_curr_label() {return curr_label;}
  void set_curr_label(int lb) {curr_label=lb;}
  int get_label_pred() {return label_pred;}
  void set_sparsez(double in_spz) {sparse_z=in_spz;}
  double get_sparsez() {return sparse_z;}
  void set_thd_db(double tdb) {thd_db=tdb;}
  double get_thd_db() {return thd_db;}
  void set_thd_pz(double tpz) {thd_pz=tpz;}
  double get_thd_pz() {return thd_pz;}
  void set_numclass(size_t ncs) {numclass=ncs;}
  size_t get_numclass() {return numclass;}
  void set_numcomp(size_t ncp) {numcomp=ncp;}
  size_t get_numcomp() {return numcomp;}
  size_t get_ncomp_gt_thd() {return ncomp_gt_thd;}
  void set_max_iter_plca(size_t mip) {max_iter_plca=mip;}
  size_t get_max_iter_plca() {return max_iter_plca;}
  size_t get_max_itertau() {return max_itertau;}
  void set_max_itertau(size_t mit) {max_itertau=mit;}
  void set_max_iter_cw(size_t mic) {max_iter_cw=mic;}
  size_t get_max_iter_cw() {return max_iter_cw;}
  void set_seglen(size_t sl) {seglen=sl;}
  size_t get_seglen() {return seglen;}
  vector< datablk >& get_extracted_comps() {return extracted_comps;}
  vector< vector<datablk> >& get_class_comps() {return class_comps;}
  
  //Notice: in this function, the pointer to the data inside the datablk is copied
  void set_class_comps(const vector< vector<datablk> >& c) {class_comps=c;}

  complex<double>* get_STFT_cpx() {return STFT_cpx;}
  void set_STFT_cpx(complex<double>* p) {STFT_cpx=p;}
  double* get_STFT_double() {return STFT_double;}
  void set_STFT_double(double* p) {STFT_double=p;}
  void set_opt_get_cw(int p) {opt_get_cw=p;}
  int get_opt_get_cw() {return opt_get_cw;}

  double* get_ptz() {return ptz;}
  double* get_pfz() {return pfz;}
  void set_pfz(double* p) {pfz=p;}
  double* get_pz() {return pz;}
  size_t* get_numz() {return numz;}
  double* get_pts() {return pts;}
  void set_pts(double* p) {pts=p;}
  double** get_ptzs() {return ptzs;}
  void set_ptzs(double** p) {ptzs=p;}
 
  
private:
  complex<double>* data;
  int datasize;
  double fz;
  int fftN;
  int fftW;
  int fftH;
  int lenf;
  int lent;
  int curr_label;
  int label_pred;
  double sparse_z;
  double thd_db; //threshold on db
  double thd_pz; //threshold on pz
  size_t numclass;
  size_t numcomp; //number of components used in plca
  size_t ncomp_gt_thd; //number of components with possibility greater than threshold
  size_t max_iter_plca;
  size_t max_itertau;
  size_t max_iter_cw; //max iteration to get compweight
  size_t seglen; //seg length for each seg to do plca, if 0(default), then use the whole data to do plca
  size_t numseg; //number of segs, lent/seglen, Notice: it is the floor of lent/seglen, so if seglen!=0, the size of ptz should be seglen*numseg*numcomp
  //extracted components for each class, seg is the components for the 
  vector< datablk > extracted_comps;

  // the outside vector<..> is for different class, the inside vector<double*> is for the components from this class, and doulbe* is the data for each instant
  vector< vector<datablk> > class_comps;
  complex<double>* STFT_cpx;
  double* STFT_double;
  double* ptz;
  double* pfz;
  double* pz;
  size_t* numz;
  double* pts;
  double** ptzs;
  int opt_get_cw;
};



#endif
