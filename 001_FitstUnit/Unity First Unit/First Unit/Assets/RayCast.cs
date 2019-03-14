using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class RayCast : MonoBehaviour {
    public Material obj;

	void clickChange () {
        Ray mouseRay = Camera.main.ScreenPointToRay(Input.mousePosition);
        RaycastHit hit;

        if (Physics.Raycast(mouseRay, out hit, 100))
        {
            hit.collider.GetComponent<MeshRenderer>().material = obj;
        }
	}

    private void Update()
    {
        if (Input.GetAxis("Fire1") != 0)
        {
            clickChange();
        }
    }
}
