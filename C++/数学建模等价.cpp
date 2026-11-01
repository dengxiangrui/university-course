#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

class MarkovModel {
private:
    vector<vector<int>> sequences;
    int n_states;
    vector<vector<int>> count_matrix;
    vector<int> row_sums;
    vector<vector<double>> transition_matrix;
    
    vector<vector<double>> P; // 人口转移矩阵
    vector<double> X0; // 初始状态
    vector<double> city_ratios;
    vector<double> suburb_ratios;
    int years;
    
public:
    MarkovModel() {
        // 初始化状态序列
        sequences = {
            {4, 3, 2, 1, 4, 3, 1, 1, 2, 3},
            {2, 1, 2, 3, 4, 4, 3, 3, 1, 1},
            {1, 3, 3, 2, 1, 2, 2, 2, 4, 4},
            {2, 3, 2, 3, 1, 1, 2, 4, 3, 1}
        };
        
        n_states = 4;
        count_matrix = vector<vector<int>>(n_states, vector<int>(n_states, 0));
        row_sums = vector<int>(n_states, 0);
        transition_matrix = vector<vector<double>>(n_states, vector<double>(n_states, 0.0));
        
        // 人口模型初始化
        P = {{0.94, 0.06}, {0.02, 0.98}};
        X0 = {0.3, 0.7};
        years = 20;
        city_ratios = vector<double>(years + 1, 0.0);
        suburb_ratios = vector<double>(years + 1, 0.0);
    }
    
    void problem1() {
        cout << "=== 问题1:一步状态转移矩阵计算 ===" << endl;
        
        // 统计转移次数
        for (const auto& seq : sequences) {
            for (size_t i = 0; i < seq.size() - 1; i++) {
                int current_state = seq[i] - 1;
                int next_state = seq[i + 1] - 1;
                count_matrix[current_state][next_state]++;
            }
        }
        
        // 计算每行总和
        for (int i = 0; i < n_states; i++) {
            row_sums[i] = 0;
            for (int j = 0; j < n_states; j++) {
                row_sums[i] += count_matrix[i][j];
            }
        }
        
        // 计算转移概率矩阵
        for (int i = 0; i < n_states; i++) {
            if (row_sums[i] > 0) {
                for (int j = 0; j < n_states; j++) {
                    transition_matrix[i][j] = static_cast<double>(count_matrix[i][j]) / row_sums[i];
                }
            }
        }
        
        // 输出结果
        cout << "转移计数矩阵:" << endl;
        printMatrix(count_matrix);
        
        cout << "\n每行和 n_i:" << endl;
        for (int i = 0; i < n_states; i++) {
            cout << row_sums[i] << " ";
        }
        cout << endl;
        
        cout << "\n估计的一步状态转移矩阵:" << endl;
        printMatrix(transition_matrix);
    }
    
    void problem2() {
        cout << "\n=== 问题2:城市和郊区人口比例变化 ===" << endl;
        
        // 存储初始值
        city_ratios[0] = X0[0];
        suburb_ratios[0] = X0[1];
        
        // 逐年计算
        vector<double> X_current = X0;
        for (int year = 1; year <= years; year++) {
            X_current = matrixVectorMultiply(P, X_current);
            city_ratios[year] = X_current[0];
            suburb_ratios[year] = X_current[1];
        }
        
        // 计算10年后的状态向量
        vector<double> X10 = X0;
        for (int i = 0; i < 10; i++) {
            X10 = matrixVectorMultiply(P, X10);
        }
        
        cout << "\n10年后的城市和郊区比例:" << endl;
        cout << fixed << setprecision(4);
        cout << "城市: " << X10[0] << " (" << X10[0] * 100 << "%)" << endl;
        cout << "郊区: " << X10[1] << " (" << X10[1] * 100 << "%)" << endl;
        
        // 计算稳态比例
        double steady_city = 0.25;
        double steady_suburb = 0.75;
        
        cout << "\n最终稳定状态的比例:" << endl;
        cout << "城市: " << steady_city << " (" << steady_city * 100 << "%)" << endl;
        cout << "郊区: " << steady_suburb << " (" << steady_suburb * 100 << "%)" << endl;
        
        // 生成绘图数据
        generatePlotData();
    }
    
private:
    template<typename T>
    void printMatrix(const vector<vector<T>>& matrix) {
        for (const auto& row : matrix) {
            for (const auto& elem : row) {
                cout << elem << " ";
            }
            cout << endl;
        }
    }
    
    vector<double> matrixVectorMultiply(const vector<vector<double>>& matrix, 
                                       const vector<double>& vector) {
        std::vector<double> result(matrix.size(), 0.0);
        for (size_t i = 0; i < matrix.size(); i++) {
            for (size_t j = 0; j < matrix[i].size(); j++) {
                result[i] += matrix[i][j] * vector[j];
            }
        }
        return result;
    }
    
    void generatePlotData() {
        // 生成人口比例数据文件
        ofstream data_file("population_data.txt");
        data_file << fixed << setprecision(4);
        data_file << "年份\t城市比例\t郊区比例\n";
        for (int year = 0; year <= years; year++) {
            data_file << year << "\t" << city_ratios[year] * 100 
                     << "\t" << suburb_ratios[year] * 100 << "\n";
        }
        data_file.close();
        
        // 生成转移矩阵数据文件
        ofstream matrix_file("transition_matrix.txt");
        matrix_file << "状态\t城市\t郊区\n";
        matrix_file << "城市\t" << P[0][0] << "\t" << P[0][1] << "\n";
        matrix_file << "郊区\t" << P[1][0] << "\t" << P[1][1] << "\n";
        matrix_file.close();
        
        // 生成对比数据文件
        ofstream compare_file("comparison_data.txt");
        compare_file << "地区\t初始状态\t10年后\t稳态\n";
        compare_file << "城市\t" << X0[0] * 100 << "\t" 
                    << city_ratios[10] * 100 << "\t" << 25.0 << "\n";
        compare_file << "郊区\t" << X0[1] * 100 << "\t" 
                    << suburb_ratios[10] * 100 << "\t" << 75.0 << "\n";
        compare_file.close();
        
        // 生成GNUplot脚本
        generateGNUplotScript();
        
        cout << "\n数据文件已生成,可以使用GNUplot进行可视化。" << endl;
        cout << "运行命令: gnuplot plot_script.plt" << endl;
    }
    
    void generateGNUplotScript() {
        ofstream script_file("plot_script.plt");
        
        script_file << "set terminal pngcairo size 1200,800 enhanced font 'Arial,12'\n";
        script_file << "set output 'markov_analysis.png'\n";
        script_file << "set multiplot layout 2,2 title '马尔可夫链分析结果' font 'Arial,14'\n\n";
        
        // 子图1：人口比例变化
        script_file << "set title '城市和郊区人口比例随时间变化'\n";
        script_file << "set xlabel '年份'\n";
        script_file << "set ylabel '人口比例 (%)'\n";
        script_file << "set xrange [0:20]\n";
        script_file << "set grid\n";
        script_file << "plot 'population_data.txt' using 1:2 with lines linewidth 2 title '城市人口比例', \\\n";
        script_file << "     'population_data.txt' using 1:3 with lines linewidth 2 title '郊区人口比例', \\\n";
        script_file << "     25 with lines linetype 2 linecolor 'blue' title '城市稳态比例', \\\n";
        script_file << "     75 with lines linetype 2 linecolor 'red' title '郊区稳态比例'\n\n";
        
        // 子图2：转移概率矩阵热力图
        script_file << "set title '转移概率矩阵'\n";
        script_file << "set xlabel ''\n";
        script_file << "set ylabel ''\n";
        script_file << "set xtics ('城市' 0, '郊区' 1)\n";
        script_file << "set ytics ('城市' 0, '郊区' 1)\n";
        script_file << "set cbrange [0:1]\n";
        script_file << "set palette defined (0 'white', 1 'blue')\n";
        script_file << "plot 'transition_matrix.txt' using 2:1:3 with image pixels notitle, \\\n";
        script_file << "     'transition_matrix.txt' using 2:1:(sprintf('%.2f', $3)) with labels font 'Arial,12' textcolor 'white' notitle\n\n";
        
        // 子图3：人口比例对比
        script_file << "set title '人口比例对比'\n";
        script_file << "set xlabel '地区'\n";
        script_file << "set ylabel '人口比例 (%)'\n";
        script_file << "set style data histogram\n";
        script_file << "set style histogram cluster gap 1\n";
        script_file << "set style fill solid 0.8\n";
        script_file << "set boxwidth 0.8\n";
        script_file << "plot 'comparison_data.txt' using 2:xtic(1) title '初始状态', \\\n";
        script_file << "     '' using 3 title '10年后', \\\n";
        script_file << "     '' using 4 title '稳态'\n\n";
        
        script_file << "unset multiplot\n";
        script_file.close();
    }
};

int main() {
    MarkovModel model;
    
    // 执行问题1
    model.problem1();
    
    // 执行问题2
    model.problem2();
    
    return 0;
}