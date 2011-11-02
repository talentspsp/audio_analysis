#ifndef _AUDIO_ANALYZER_H
#define _AUDIO_ANALYZER_H
#include "spuc/generic/complex.h"
#include <vector>
#include "fastmath.h"

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
  int readindata(const char* filename, int label=-1); // if fail, return negative value

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

  //extract H functions with plca or dirichlet plca with given global W
  bool extract_adapted_time_functions();

  //this group of functions used to get and set parameters
  complex<double>* get_data() {return data;}
  void set_data(complex<double>* in_data) {data=in_data;}
  int get_datasize() {return datasize;}
  double get_fz() {return fz;}
  void set_fft_param(int in_fftN, int in_fftW, int in_fftH) {fftN=in_fftN; fftW=in_fftW; fftH=in_fftH;}
  int get_lenf() {return lenf;}
  int get_lent() {return lent;}
  size_t get_lent_gt_thd() {return lent_gt_thd;}
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
  void set_numcomp_per_seg(size_t ncp) {numcomp_per_seg=ncp;}
  size_t get_numcomp_per_seg() {return numcomp_per_seg;}
  size_t get_numcomp_all() {return numcomp_all;}
  size_t get_ncomp_gt_thd() {return ncomp_gt_thd;}
  void set_max_iter_plca(size_t mip) {max_iter_plca=mip;}
  size_t get_max_iter_plca() {return max_iter_plca;}
  size_t get_max_itertau() {return max_itertau;}
  void set_max_itertau(size_t mit) {max_itertau=mit;}
  void set_max_iter_cw(size_t mic) {max_iter_cw=mic;}
  size_t get_max_iter_cw() {return max_iter_cw;}
  void set_seglen_sec(size_t sl) {seglen_sec=sl;}
  size_t get_seglen_sec() {return seglen_sec;}
  void set_seglen_frame(size_t sl) {seglen_frame=sl;}
  size_t get_seglen_frame() {return seglen_frame;}
  size_t get_numseg() {return numseg;}
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
  double* get_class_prob() {return class_prob;}

  void set_dirich(int d) {dirich=d;}
  int get_dirich() {return dirich;}

  void set_dp(double in_dp) {dp=in_dp;}
  double get_dp() {return dp;}

  void set_esz(double e) {esz=e;}
  double get_esz() {return esz;}

  FMmatrix<double> get_ew() {return estimated_w;}
  FMmatrix<double> get_eh() {return estimated_h;}
  FMmatrix<double> get_ez() {return estimated_z;}
  
private:
  complex<double>* data;
  int datasize;
  double fz;
  int fftN;
  int fftW;
  int fftH;
  int lenf;
  int lent;
  size_t lent_gt_thd; //number of frames whose powers are greater than the threshold
  int curr_label;
  int label_pred;
  double sparse_z;
  double esz; //sparseness prior used in extract_adapted_time_functions, default: 0.2
  double thd_db; //threshold on db
  double thd_pz; //threshold on pz
  int dirich; //if use dirichlet plca in extract_adapted_time_functions,0: use plca 1: use dirichlet plca; default: 1
  double dp;//dirichlet prior, default: 0.01

  size_t numclass;
  size_t numcomp_per_seg; //number of components used in plca
  size_t numcomp_all; //number of components from all the segments
  size_t ncomp_gt_thd; //number of components with possibility greater than threshold
  size_t max_iter_plca;
  size_t max_itertau;
  size_t max_iter_cw; //max iteration to get compweight
  size_t seglen_sec; //seg length for each seg to do plca, if 0(default), then use the whole data to do plca
  size_t seglen_frame; //number of frames of each segment, seglen_frame=seglen_sec*fz/fftH
  size_t numseg; //number of segs, lent/seglen_frame, Notice: it is the floor of lent/seglen_frame, so if seglen!=0, the size of ptz should be seglen_frame*numseg*numcomp_per_seg
  //extracted components for each class, seg is the components for the 
  vector< datablk > extracted_comps;

  // the outside vector<..> is for different class, the inside vector<double*> is for the components from this class, and double* is the data for each instant
  vector< vector<datablk> > class_comps;
  complex<double>* STFT_cpx;
  double* STFT_double;
  double* STFT_double_gt_thd; //STFT result in which each column's mean power is greater than the threshold
  double* ptz;
  double* pfz;
  double* pz;
  size_t* numz;
  double* pts;
  double** ptzs;
  double* class_prob;
  int opt_get_cw;
  FMmatrix<double> estimated_w;
  FMmatrix<double> estimated_h;
  FMmatrix<double> estimated_z;
};



#endif
