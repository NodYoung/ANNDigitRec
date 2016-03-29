#ANNDigitRec
 
**This is a Digital Recegtion demo based on Neural network.**

**Platform: VS2010 + Opencv2.4.8**

当初好奇就想做个试试，人懒，实现基本功能后就一直没有再做完善。待日后吧，也可能没有以后。

先大概介绍一下基本思路：

**1.**TrainData是从其他地方找的，直接就是normalized过的。每个数字只有5个样本。TestData里的图片样本来自于图片'scanDigit.bmp',需要进行preprocess，这部分工作在'preprocess.cpp'中。关于其详细信息，欢迎参考[http://blog.csdn.net/nnnnnnnnnnnny/article/details/51004561](http://blog.csdn.net/nnnnnnnnnnnny/article/details/51004561)。

**2.**有了样本之后，我们需要从样本中提取特征信息，当然神经网络里可以把整幅图片的数据直接送进去。这里我们为了降低处理的复杂度，对图像进行重采样（当然也可以使用PCA之类的降维方法），将64x32的图像采样到16x8的分辨率。project中'getTestData.cpp'和'getTrainData.cpp'即将这些特征及对应标签保存到了txt文件中（注意'testLabel.txt'中标签数据是我手动录入的）。

**3.**有了这些数据，我们就可以利用ANN进行训练并识别了，详见'neuralNetwork.cpp'。关于Opencv中neural network的使用方法可参见博客[http://blog.csdn.net/nnnnnnnnnnnny/article/details/50973583](http://blog.csdn.net/nnnnnnnnnnnny/article/details/50973583)

**4.**其实最后效果不是很好，我只简单跑了一下，准确率才60%多，（如前所述，人懒，呵呵，也没有什么实际应用）没继续调。

**5.**注意，project中的代码并没有合在一起，每个.cpp都是单独运行的。
