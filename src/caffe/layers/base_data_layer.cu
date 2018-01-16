#include <vector>

#include "caffe/layers/base_data_layer.hpp"

namespace caffe {

template <typename Dtype>
void BasePrefetchingDataLayer<Dtype>::Forward_gpu(
    const vector<Blob<Dtype>*>& bottom, const vector<Blob<Dtype>*>& top) {
  if (prefetch_current_) {
    prefetch_free_.push(prefetch_current_);
  }
  prefetch_current_ = prefetch_full_.pop("Waiting for data");
  // Reshape to loaded data.
  top[0]->ReshapeLike(prefetch_current_->data_);
  top[0]->set_gpu_data(prefetch_current_->data_.mutable_gpu_data());
  if (this->output_labels_) {
    if (this->box_label_) {
      for (int i = 0; i < top.size() - 1; ++i) {
        top[i+1]->ReshapeLike(*(prefetch_current_->multi_label_[i]));
	top[i+1]->set_gpu_data(prefetch_current_->multi_label_[i]->mutable_gpu_data());        
      }
    } else {
      // Reshape to loaded labels.
      top[1]->ReshapeLike(prefetch_current_->label_);
      top[1]->set_gpu_data(prefetch_current_->label_.mutable_gpu_data());
    }
  }
}

INSTANTIATE_LAYER_GPU_FORWARD(BasePrefetchingDataLayer);

}  // namespace caffe
