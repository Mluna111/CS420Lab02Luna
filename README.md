<h1>Parallel Histogram Calculation</h1>

<h2>Description</h2>
<p>This project demonstrates the parallel calculation of a histogram using C++ and multithreading. The program reads a binary file into memory, calculates the histogram of byte values, and outputs the results. It performs the calculation twice: once using a global histogram and once using local histograms for each thread.</p>

<h2>Files in the Project</h2>
<ul>
    <li><strong>main.cpp</strong>: The main file containing the implementation of the histogram calculation using multithreading.</li>
    <li><strong>AllocLocalHistogram.h</strong>: A header file providing functions to allocate and deallocate 2D arrays.</li>
</ul>

<h2>Libraries Used</h2>
<ul>
    <li><strong>&lt;thread&gt;</strong>: Used for creating and managing multiple threads.</li>
    <li><strong>&lt;algorithm&gt;</strong>: Provides functions like <code>for_each</code> for algorithm operations.</li>
    <li><strong>&lt;fstream&gt;</strong>: Used for file input and output operations.</li>
    <li><strong>&lt;iostream&gt;</strong>: Provides basic input and output services.</li>
    <li><strong>&lt;vector&gt;</strong>: Used for handling dynamic arrays.</li>
    <li><strong>&lt;mutex&gt;</strong>: Provides mutual exclusion for thread synchronization.</li>
</ul>

<h2>How to Run</h2>
<p>Follow these steps to run the project:</p>
<ol>
    <li>Ensure you have a C++ compiler installed, such as g++.</li>
    <li>Compile the project using the following command:</li>
    <pre><code>g++ -o histogram main.cpp -pthread</code></pre>
    <li>Run the executable with the binary file as an argument:</li>
    <pre><code>./histogram &lt;binary_file&gt;</code></pre>
</ol>

    <h2>Input</h2>
    <p>The program expects a binary file as input, which will be processed to calculate the histogram.</p>

    <h2>Output</h2>
    <p>The program outputs the calculated histogram values for each byte (0-255). It will display the histogram twice: first using a global histogram and then using local histograms for each thread.</p>
