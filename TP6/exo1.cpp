#include "tp6.h"
#include <QApplication>
#include <time.h>

MainWindow* w = nullptr;

void Graph::buildFromAdjenciesMatrix(int **adjacencies, int nodeCount)
{
	/**
	  * Make a graph from a matrix
	  * first create all nodes, add it to the graph then connect them
	  * this->appendNewNode
	  * this->nodes[i]->appendNewEdge
	  */
	for(size_t i = 0; i < nodeCount; i++)
	{
		GraphNode* node = new GraphNode(i);
		this->appendNewNode(node);
	}

	for(size_t i = 0; i < nodeCount; i++)
	{
		for(size_t j = 0; j < nodeCount; j++)
		{
			if(adjacencies[i][j] != 0)
			{
				this->nodes[i]->appendNewEdge(nodes[j], adjacencies[i][j]);
			}
		}
	}
}

void Graph::deepTravel(GraphNode *first, GraphNode *nodes[], int &nodesSize, bool visited[])
{
	/**
	  * Fill nodes array by travelling graph starting from first and using recursivity
	  */
	// mark that the note is visited
	visited[first->value] = true;
	nodes[nodesSize++] = first;

	for(Edge* currentEdge = first->edges; currentEdge != NULL; currentEdge = currentEdge->next)
	{
		if(!visited[currentEdge->destination->value])
			deepTravel(currentEdge->destination, nodes, nodesSize, visited);
	}

}

void Graph::wideTravel(GraphNode *first, GraphNode *nodes[], int &nodesSize, bool visited[])
{
	/**
	 * Fill nodes array by travelling graph starting from first and using queue
	 * nodeQueue.push(a_node)
	 * nodeQueue.front() -> first node of the queue
	 * nodeQueue.pop() -> remove first node of the queue
	 * nodeQueue.size() -> size of the queue
	 */
	std::queue<GraphNode*> nodeQueue;
	nodeQueue.push(first);
}

bool Graph::detectCycle(GraphNode *first, bool visited[])
{
	/**
	  Detect if there is cycle when starting from first
	  (the first may not be in the cycle)
	  Think about what's happen when you get an already visited node
	**/
    return false;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainWindow::instruction_duration = 150;
	w = new GraphWindow();
	w->show();

	return a.exec();
}
