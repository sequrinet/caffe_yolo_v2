#ifndef CAFFE_DATA_LAYER_HPP_
#define CAFFE_DATA_LAYER_HPP_

#include <vector>

#include "caffe/blob.hpp"
//#include "caffe/data_reader.hpp"
#include "caffe/data_transformer.hpp"
#include "caffe/internal_thread.hpp"
#include "caffe/layer.hpp"
#include "caffe/layers/base_data_layer.hpp"
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"

namespace caffe {

template <typename Dtype>
class BoxDataLayer : public BasePrefetchingDataLayer<Dtype> {
 public:
  explicit BoxDataLayer(const LayerParameter& param);
  virtual ~BoxDataLayer();
  virtual void DataLayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top);
  // DataLayer uses DataReader instead for sharing for parallelism
  virtual inline bool ShareInParallel() const { return false; }
  virtual inline const char* type() const { return "BoxData"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 100; }

  void transform_label(int count, Dtype* top_label, const vector<BoxLabel>& box_labels, int side);

 protected:
  void Next();
  virtual void load_batch(Batch<Dtype>* batch);

  //DataReader reader_;
  shared_ptr<db::DB> db_;
  shared_ptr<db::Cursor> cursor_;
  uint64_t offset_;
  vector<int> sides_;
};

}  // namespace caffe

#endif  // CAFFE_DATA_LAYER_HPP_
