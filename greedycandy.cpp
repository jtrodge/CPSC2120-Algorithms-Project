#include <bits/stdc++.h>

using namespace std;

struct Candy {
    int weight = 0;
    int value = 0;
    //Candy(int w, int v) {weight = w; value = v;}
};

int best_sol = 0;
int curWeight[3] = {0, 0, 0};

bool cmp(struct Candy a, struct Candy b) {
    double c1 = (double)a.value / (double)a.weight;
    double c2 = (double)b.value / (double)b.weight;
    return c1 > c2;
    //return a.density > b.density
}

int greedyknapsack(struct Candy arr[], int b[], int n) { //int b1, int b2, int b3
    sort(arr, arr + n, cmp); //sort in descending order
    int curWeight[3] = {0, 0, 0};
    int finalvalue = 0;

    for (int i = 0; i < n; i++) {
        //Check if bag is full; if it is full, move on the next bag
        if (curWeight[0] + arr[i].weight <= b[0]) {
            curWeight[0] += arr[i].weight;
            finalvalue += arr[i].value;
        }
        else if (curWeight[1] + arr[i].weight <= b[1]) {
            curWeight[1] += arr[i].weight;
            finalvalue += arr[i].value;
        }
        else if (curWeight[2] + arr[i].weight <= b[2]) {
            curWeight[2] += arr[i].weight;
            finalvalue += arr[i].value;
        }
    }
    return finalvalue;
}

int greedyfill(struct Candy arr[], int b[], int n) {
  int curWeight[3] = {0, 0, 0};
  int finalvalue = 0;
  for (int i = 0; i < n; i++) {
      //Check if bag is full; if it is full, move on the next bag
      if (curWeight[0] + arr[i].weight <= b[0]) {
          curWeight[0] += arr[i].weight;
          finalvalue += arr[i].value;
      }
      else if (curWeight[1] + arr[i].weight <= b[1]) {
          curWeight[1] += arr[i].weight;
          finalvalue += arr[i].value;
      }
      else if (curWeight[2] + arr[i].weight <= b[2]) {
          curWeight[2] += arr[i].weight;
          finalvalue += arr[i].value;
      }
  }
  return finalvalue;
}

void greedyunfill(struct Candy arr[], int b[], int n) {
  int curWeight[3] = {0, 0, 0};
  int finalvalue = 0;
  for (int i = 0; i < n; i++) {
      //Check if bag is full; if it is full, move on the next bag
      if (curWeight[0] + arr[i].weight <= b[0]) {
          curWeight[0] -= arr[i].weight;
          finalvalue -= arr[i].value;
      }
      else if (curWeight[1] + arr[i].weight <= b[1]) {
          curWeight[1] -= arr[i].weight;
          finalvalue -= arr[i].value;
      }
      else if (curWeight[2] + arr[i].weight <= b[2]) {
          curWeight[2] -= arr[i].weight;
          finalvalue -= arr[i].value;
      }
  }
}

//Refine solution: Take a candy from one bag and move it to another bag
//Move candy from bag x to bag y
//Better solution: "repair" bags x and y
//remove things from bag y
//Refill bag x
int iterativeknapsack(struct Candy arr[], int b[], int n, int T) {
  int *norm_sol = new int[T];
  norm_sol[0] = greedyknapsack(arr, b, n);
  random_shuffle(arr, arr + n); // Shuffle weight and value randomly
  for (int i = 0; i < T; i++) {
    for (int j = 0; j < 3 % T; j++) {
      if (best_sol < norm_sol[i]) {
        best_sol = norm_sol[i];
      }
      if (b[j] >= 2000) {
        norm_sol[i] = greedyfill(arr, b, n);
        random_shuffle(arr, arr + n);
      }
      if (b[j] <= 2000) {
        greedyunfill(arr, b, n);
      }
    }
  }
  return best_sol;
}
int upperBound(struct Candy arr[], int b[], int index, int n) {
  int sum = 0;
  int sum_value = 0;
  int totalWeight = 0;
  for (int i = index; i < n; i++) {
    sum = sum + arr[i].weight;
    sum_value = sum_value + arr[i].value;
    totalWeight = curWeight[0] + curWeight[1] + curWeight[2];
    // Add up until 6000
    if (sum > (6000 - totalWeight)) {
      break;
    }
  }
  return sum_value;
}
int fv = 0;
// Try all possible solutions for item #1 (weight, value)
// Recursively try the rest of the items
void pruneknapsack(struct Candy arr[], int b[], int i, int n) {
  if (i == n) { // base case
    if (fv > best_sol) {
      best_sol = fv;
    }
    return;
  }
  // Pruning
  if (upperBound(arr, b, i, n) + fv < best_sol) {
    return;
  }
  for (int j = 0; j < 3; j++) {
    if (curWeight[j] + arr[i].weight <= b[j]) {
      curWeight[j] += arr[i].weight;
      fv += arr[i].value;
      pruneknapsack(arr, b, i+1, n); // recurse for optimal solution
      curWeight[j] -= arr[i].weight;
      fv -= arr[i].value;
    }
  }
  pruneknapsack(arr, b, i+1, n); // Case for unused candy

}

int main() {
  ifstream inputfile("candy.txt");
  int weight;
  int value;
  int storeW[20];
  int storeV[20];
  for (int i = 0; i < 20; i++) {
    storeW[i] = 0;
    storeV[i] = 0;
  }
  int numlines = 0;
  //Store weight and value of candy in an array
  while (inputfile >> weight >> value) {
    storeW[numlines] = weight;
    storeV[numlines] = value;
    numlines++;
  }
  // Initialize candy array
  struct Candy *candyArray;
  candyArray = new Candy[numlines];

  for (int i = 0; i < numlines; i++) {
    candyArray[i].weight = storeW[i];
    candyArray[i].value = storeV[i];
  }
  int *bag = new int[3];
  for (int i = 0; i < 3; i++) {
    bag[i] = 2000;
  }
  //call the function
  cout << "Greedy = " << greedyknapsack(candyArray, bag, numlines) << endl;
  cout << "Refinement = " << iterativeknapsack(candyArray, bag, numlines, 1000) << endl;
  sort(candyArray, candyArray + numlines, cmp); //sort in descending order
  pruneknapsack(candyArray, bag, 0, numlines);
  cout << "Exhaustive = " << best_sol << endl;
  return 0;
}
