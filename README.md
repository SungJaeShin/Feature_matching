# Feature Matching
## Applied Feature Extraction and Descriptor Method to robust matching !!
**[Goal] We can apply various extracton and descriptor method for matching in challenging environment**
- Challenging Environment 
   <table>
      <tr>
         <td> Query Image</td>
         <td> Candidate Image </td>
      </tr> 
      <tr>
         <td><img src="./query.png"/> </td>
         <td><img src="./cand.png"/> </td>
      </tr>
   </table>

## 1. Dependencies
   - OpenCV 3.2.0, OpenCV Contrib 3.2.0

## 2. Methodology to apply feature matching
   - Feature Extraction Method
      - [1] ORB 
      - [2] goodFeaturesToTrack 
      - [3] SIFT
      
   - Feature Descriptor Method
      - [1] AKAZE
      - [2] SURF 
      - [3] DAISY

   - Outlier Rejection Method
      - [1] findFundamentalMatrix (FM)

## 3. Build and Run 
Clone the repository and build and run simultaneously:
```
   cd ${workspace}
   git clone https://github.com/SungJaeShin/Feature_matching.git
   cd Feature_matching
   sh start.sh
```

## 4. Changing Parameters
- **IMAGE_NUM** (include.h): Put value to test image pair  
- **EXTRACT_MODE** (include.h):
   - 1 &rarr; apply ORB
   - 2 &rarr; apply GFTT (GoodFeaturesToTrack)
   - 3 &rarr; apply SIFT
   - 4 &rarr; apply GFTT(Query) + ORB(Candidate) 
   - 5 &rarr; apply GFTT(Query) + SIFT(Candidate)
- **DESCRIPTOR_MODE** (include.h): 
   - 1 &rarr; apply AKAZE
   - 2 &rarr; apply SURF
   - 3 &rarr; apply DAISY
- **MATCHER_MODE** (include.h):
   - 1 &rarr; apply BF Matcher (Bruth-Force)
   - 2 &rarr; apply FLANN Matcher (Fast Library for Approximate Nearest Neighbors)
- **MATCHING_MODE** (include.h):
   - 1 &rarr; apply vanilla match
   - 2 &rarr; apply knn match
   - 3 &rarr; apply radius match
- **MIN_DIS** (include.h): Put value to change feature's Euclidean distance when using GoodFeaturesToTrack extractor
- **query_path, cand_path** (matching.cpp): Put absolute path for getting query image and candidate image
- **save_path** (plot.h): Put absolute path for saving matching result 

## 5. Results 
   <table>
      <tr>
         <td> ORB + AKAZE + FM (inliers 22)</td>
         <td> ORB + SURF + FM (inliers 44)</td>
         <td> ORB + DAISY + FM (inliers 57)</td>
      </tr> 
      <tr>
         <td><img src="./results/3_final_orb_akaze_result.png"/> </td>
         <td><img src="./results/3_final_orb_surf_result.png"/> </td>
         <td><img src="./results/3_final_orb_daisy_result.png"/> </td>
      </tr>
      <tr>
         <td> GFTT + AKAZE + FM (inliers 10)</td>
         <td> GFTT + SURF + FM (inliers 9)</td>
         <td> GFTT + DAISY + FM (inliers 10)</td>
      </tr> 
      <tr>
         <td><img src="./results/3_final_gftt_akaze_result.png"/> </td>
         <td><img src="./results/3_final_gftt_surf_result.png"/> </td>
         <td><img src="./results/3_final_gftt_daisy_result.png"/> </td>
      </tr>
      <tr>
         <td> SIFT + AKAZE + FM (inliers 11)</td>
         <td> SIFT + SURF + FM (inliers 17)</td>
         <td> SIFT + DAISY + FM (inliers 19)</td>
      </tr> 
      <tr>
         <td><img src="./results/3_final_sift_akaze_result.png"/> </td>
         <td><img src="./results/3_final_sift_surf_result.png"/> </td>
         <td><img src="./results/3_final_sift_daisy_result.png"/> </td>
      </tr>
      <tr>
         <td> GFTT + ORB + AKAZE + FM (inliers 10)</td>
         <td> GFTT + ORB + SURF + FM (inliers 10)</td>
         <td> GFTT + ORB + DAISY + FM (inliers 8)</td>
      </tr> 
      <tr>
         <td><img src="./results/3_final_gftt_orb_akaze_result.png"/> </td>
         <td><img src="./results/3_final_gftt_orb_surf_result.png"/> </td>
         <td><img src="./results/3_final_gftt_orb_daisy_result.png"/> </td>
      </tr>
      <tr>
         <td> GFTT + SIFT + AKAZE + FM (inliers 10)</td>
         <td> GFTT + SIFT + SURF + FM (inliers 9)</td>
         <td> GFTT + SIFT + DAISY + FM (inliers 9)</td>
      </tr> 
      <tr>
         <td><img src="./results/3_final_gftt_sift_akaze_result.png"/> </td>
         <td><img src="./results/3_final_gftt_sift_surf_result.png"/> </td>
         <td><img src="./results/3_final_gftt_sift_daisy_result.png"/> </td>
      </tr>
   </table>

## 6. Best Results
   - ORB + DAISY + FM (inliers 57)
      <img src="./results/3_final_orb_daisy_result.png"/>

## 7. Future Works
   - Apply SuperPoint & SuperGlue
   - Apply GCBB (Linear time vehicle relocation in SLAM paper)
   - Apply time consumption
   - Apply VFC outlier rejection (Robust Point Matching via Vector Field Consensus)
